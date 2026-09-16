#include "nav/GpsdClient.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QtMath>

GpsdClient::GpsdClient(QObject *parent)
    : QObject(parent)
{
    connect(&m_socket, &QTcpSocket::connected, this, &GpsdClient::onConnected);
    connect(&m_socket, &QTcpSocket::disconnected, this, &GpsdClient::onDisconnected);
    connect(&m_socket, &QTcpSocket::readyRead, this, &GpsdClient::onReadyRead);
    connect(&m_socket, &QAbstractSocket::errorOccurred, this, &GpsdClient::onError);

    m_reconnect.setInterval(3000);
    m_reconnect.setSingleShot(true);
    connect(&m_reconnect, &QTimer::timeout, this, &GpsdClient::tryReconnect);

    m_simTimer.setInterval(200);
    connect(&m_simTimer, &QTimer::timeout, this, &GpsdClient::simTick);
}

void GpsdClient::setHost(const QString &host, quint16 port)
{
    m_host = host;
    m_port = port;
}

void GpsdClient::connectToDaemon()
{
    m_forceSim = false;
    m_simTimer.stop();
    tryReconnect();
}

void GpsdClient::startSimulation()
{
    m_forceSim = true;
    m_reconnect.stop();
    m_socket.abort();
    setStatus(false, true, QStringLiteral("simulated"));
    m_simTimer.start();
}

void GpsdClient::setSimulatedPosition(double lat, double lon, double headingDeg, double speedMps)
{
    applyTpv(lat, lon, m_altitudeM, headingDeg, speedMps, 3);
}

void GpsdClient::tryReconnect()
{
    if (m_forceSim)
        return;
    setStatus(false, m_simulated, QStringLiteral("connecting to gpsd"));
    m_socket.abort();
    m_socket.connectToHost(m_host, m_port);
}

void GpsdClient::onConnected()
{
    m_simTimer.stop();
    m_buf.clear();
    m_socket.write("?WATCH={\"enable\":true,\"json\":true};\n");
    setStatus(true, false, QStringLiteral("gpsd"));
}

void GpsdClient::onDisconnected()
{
    if (m_forceSim)
        return;
    setStatus(false, true, QStringLiteral("gpsd disconnected"));
    m_simTimer.start();
    m_reconnect.start();
}

void GpsdClient::onError()
{
    if (m_socket.state() == QAbstractSocket::UnconnectedState && !m_forceSim) {
        setStatus(false, true, QStringLiteral("gpsd unavailable"));
        m_simTimer.start();
        m_reconnect.start();
    }
}

void GpsdClient::onReadyRead()
{
    m_buf += m_socket.readAll();
    while (true) {
        const int nl = m_buf.indexOf('\n');
        if (nl < 0)
            break;
        const QByteArray line = m_buf.left(nl).trimmed();
        m_buf.remove(0, nl + 1);
        const auto doc = QJsonDocument::fromJson(line);
        if (!doc.isObject())
            continue;
        const auto obj = doc.object();
        if (obj.value(QStringLiteral("class")).toString() != QLatin1String("TPV"))
            continue;
        const int mode = obj.value(QStringLiteral("mode")).toInt();
        applyTpv(obj.value(QStringLiteral("lat")).toDouble(m_latitude),
                 obj.value(QStringLiteral("lon")).toDouble(m_longitude),
                 obj.value(QStringLiteral("alt")).toDouble(m_altitudeM),
                 obj.value(QStringLiteral("track")).toDouble(m_headingDeg),
                 obj.value(QStringLiteral("speed")).toDouble(m_speedMps),
                 mode);
    }
}

void GpsdClient::simTick()
{
    m_simT += 0.2;
    // Slow eastbound crawl near UCLA so the map has a live fix without gpsd.
    const double heading = 90.0;
    const double speed = 12.0; // m/s
    const double dlat = 0.0;
    const double dlon = (speed * 0.2) / (111320.0 * qCos(qDegreesToRadians(m_latitude)));
    applyTpv(m_latitude + dlat, m_longitude + dlon, m_altitudeM, heading, speed, 3);
}

void GpsdClient::applyTpv(double lat, double lon, double alt, double heading, double speed, int mode)
{
    m_latitude = lat;
    m_longitude = lon;
    m_altitudeM = alt;
    m_headingDeg = heading;
    m_speedMps = speed;
    m_fixMode = mode;
    emit positionChanged();
}

void GpsdClient::setStatus(bool connected, bool simulated, const QString &text)
{
    const bool changed = connected != m_connected || simulated != m_simulated || text != m_statusText;
    m_connected = connected;
    m_simulated = simulated;
    m_statusText = text;
    if (changed)
        emit statusChanged();
}
