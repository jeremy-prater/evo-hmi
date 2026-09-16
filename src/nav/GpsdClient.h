#pragma once

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTimer>

// gpsd JSON client (TCP :2947). Falls back to a simulated TPV stream
// when the daemon is missing so the map screen still moves.
class GpsdClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY statusChanged)
    Q_PROPERTY(bool simulated READ simulated NOTIFY statusChanged)
    Q_PROPERTY(int fixMode READ fixMode NOTIFY positionChanged)
    Q_PROPERTY(double latitude READ latitude NOTIFY positionChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY positionChanged)
    Q_PROPERTY(double altitudeM READ altitudeM NOTIFY positionChanged)
    Q_PROPERTY(double headingDeg READ headingDeg NOTIFY positionChanged)
    Q_PROPERTY(double speedMps READ speedMps NOTIFY positionChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusChanged)

public:
    explicit GpsdClient(QObject *parent = nullptr);

    bool connected() const { return m_connected; }
    bool simulated() const { return m_simulated; }
    int fixMode() const { return m_fixMode; }
    double latitude() const { return m_latitude; }
    double longitude() const { return m_longitude; }
    double altitudeM() const { return m_altitudeM; }
    double headingDeg() const { return m_headingDeg; }
    double speedMps() const { return m_speedMps; }
    QString statusText() const { return m_statusText; }

    void setHost(const QString &host, quint16 port);
    void connectToDaemon();
    void startSimulation();
    void setSimulatedPosition(double lat, double lon, double headingDeg, double speedMps);

signals:
    void positionChanged();
    void statusChanged();

private:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError();
    void tryReconnect();
    void simTick();
    void applyTpv(double lat, double lon, double alt, double heading, double speed, int mode);
    void setStatus(bool connected, bool simulated, const QString &text);

    QTcpSocket m_socket;
    QTimer m_reconnect;
    QTimer m_simTimer;
    QByteArray m_buf;
    QString m_host = QStringLiteral("127.0.0.1");
    quint16 m_port = 2947;
    bool m_connected = false;
    bool m_simulated = true;
    bool m_forceSim = false;
    int m_fixMode = 0;
    double m_latitude = 34.0699;
    double m_longitude = -118.4452;
    double m_altitudeM = 80.0;
    double m_headingDeg = 90.0;
    double m_speedMps = 0.0;
    QString m_statusText = QStringLiteral("simulated");
    double m_simT = 0;
};
