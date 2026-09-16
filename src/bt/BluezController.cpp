#include "bt/BluezController.h"

#include "bt/DeviceModel.h"

#ifdef EVO_HMI_HAS_DBUS
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusVariant>
#endif

namespace {
const QStringList kSimTracksTitle = {
    QStringLiteral("Blacklist"),
    QStringLiteral("Insomnia"),
    QStringLiteral("Redline"),
};
const QStringList kSimTracksArtist = {
    QStringLiteral("Garage Mix"),
    QStringLiteral("Night Run"),
    QStringLiteral("Evo Club"),
};
}

BluezController::BluezController(QObject *parent)
    : QObject(parent)
    , m_devices(new DeviceModel(this))
{
    m_simTimer.setInterval(250);
    connect(&m_simTimer, &QTimer::timeout, this, &BluezController::simTick);
}

void BluezController::setForceSimulated(bool force)
{
    m_forceSim = force;
}

void BluezController::start()
{
    if (!m_forceSim && tryStartBluez())
        return;
    startSimulated();
}

bool BluezController::tryStartBluez()
{
#ifdef EVO_HMI_HAS_DBUS
    auto bus = QDBusConnection::systemBus();
    if (!bus.isConnected())
        return false;

    QDBusInterface objMgr(QStringLiteral("org.bluez"), QStringLiteral("/"),
                          QStringLiteral("org.freedesktop.DBus.ObjectManager"), bus);
    if (!objMgr.isValid())
        return false;

    const QDBusMessage reply = objMgr.call(QStringLiteral("GetManagedObjects"));
    if (reply.type() == QDBusMessage::ErrorMessage)
        return false;

    m_simulated = false;
    m_available = true;
    m_adapterName = QStringLiteral("BlueZ");
    emit availableChanged();
    emit adapterChanged();

    const auto args = reply.arguments();
    if (args.isEmpty())
        return true;

    const auto arg = args.first().value<QDBusArgument>();
    arg.beginMap();
    while (!arg.atEnd()) {
        QDBusObjectPath path;
        QMap<QString, QVariantMap> ifaces;
        arg.beginMapEntry();
        arg >> path >> ifaces;
        arg.endMapEntry();

        if (ifaces.contains(QStringLiteral("org.bluez.Adapter1"))) {
            const auto props = ifaces.value(QStringLiteral("org.bluez.Adapter1"));
            m_powered = props.value(QStringLiteral("Powered")).toBool();
            m_adapterName = props.value(QStringLiteral("Alias"), props.value(QStringLiteral("Name"))).toString();
            emit poweredChanged();
            emit adapterChanged();
        }
        if (ifaces.contains(QStringLiteral("org.bluez.Device1"))) {
            const auto props = ifaces.value(QStringLiteral("org.bluez.Device1"));
            BtDevice d;
            d.address = props.value(QStringLiteral("Address")).toString();
            d.name = props.value(QStringLiteral("Alias"), props.value(QStringLiteral("Name"))).toString();
            d.paired = props.value(QStringLiteral("Paired")).toBool();
            d.connected = props.value(QStringLiteral("Connected")).toBool();
            const auto uuids = props.value(QStringLiteral("UUIDs")).toStringList();
            d.audio = uuids.join(QLatin1Char(' ')).contains(QLatin1String("110b"), Qt::CaseInsensitive)
                || uuids.join(QLatin1Char(' ')).contains(QLatin1String("110A"), Qt::CaseInsensitive)
                || uuids.join(QLatin1Char(' ')).contains(QLatin1String("110E"), Qt::CaseInsensitive);
            if (!d.address.isEmpty())
                m_devices->upsert(d);
            if (d.connected)
                m_connectedDeviceName = d.name;
        }
        if (ifaces.contains(QStringLiteral("org.bluez.MediaPlayer1"))) {
            m_playerPath = path.path();
            const auto props = ifaces.value(QStringLiteral("org.bluez.MediaPlayer1"));
            m_playerStatus = props.value(QStringLiteral("Status")).toString();
            const auto track = props.value(QStringLiteral("Track")).toMap();
            m_trackTitle = track.value(QStringLiteral("Title")).toString();
            m_trackArtist = track.value(QStringLiteral("Artist")).toString();
            m_trackAlbum = track.value(QStringLiteral("Album")).toString();
            m_durationMs = track.value(QStringLiteral("Duration")).toLongLong();
            m_positionMs = props.value(QStringLiteral("Position")).toLongLong();
            emit playerChanged();
        }
    }
    arg.endMap();
    return true;
#else
    return false;
#endif
}

void BluezController::startSimulated()
{
    m_simulated = true;
    m_available = true;
    m_powered = true;
    m_adapterName = QStringLiteral("sim-hci0");
    m_connectedDeviceName = QStringLiteral("Jeremy's Phone");
    m_playerStatus = QStringLiteral("playing");
    m_durationMs = 210000;
    m_positionMs = 42000;
    m_simTrack = 0;
    m_trackTitle = kSimTracksTitle.at(0);
    m_trackArtist = kSimTracksArtist.at(0);
    m_trackAlbum = QStringLiteral("A2DP Demo");

    m_devices->clear();
    m_devices->upsert({QStringLiteral("AA:BB:CC:DD:EE:01"), QStringLiteral("Jeremy's Phone"), true, true, true});
    m_devices->upsert({QStringLiteral("AA:BB:CC:DD:EE:02"), QStringLiteral("Passenger iPhone"), true, false, true});
    m_devices->upsert({QStringLiteral("AA:BB:CC:DD:EE:03"), QStringLiteral("OBD Adapter"), true, false, false});

    emit availableChanged();
    emit poweredChanged();
    emit adapterChanged();
    emit playerChanged();
    m_simTimer.start();
}

void BluezController::simTick()
{
    if (!m_simulated || m_playerStatus != QLatin1String("playing"))
        return;
    m_positionMs = qMin(m_durationMs, m_positionMs + 250);
    if (m_positionMs >= m_durationMs)
        next();
    else
        emit playerChanged();
}

void BluezController::setPowered(bool powered)
{
#ifdef EVO_HMI_HAS_DBUS
    if (!m_simulated) {
        QDBusInterface adapter(QStringLiteral("org.bluez"), QStringLiteral("/org/bluez/hci0"),
                               QStringLiteral("org.freedesktop.DBus.Properties"),
                               QDBusConnection::systemBus());
        adapter.call(QStringLiteral("Set"), QStringLiteral("org.bluez.Adapter1"),
                     QStringLiteral("Powered"), QVariant::fromValue(QDBusVariant(powered)));
    }
#endif
    if (m_powered == powered)
        return;
    m_powered = powered;
    emit poweredChanged();
}

void BluezController::startDiscovery()
{
    m_discovering = true;
    emit discoveringChanged();
#ifdef EVO_HMI_HAS_DBUS
    if (!m_simulated) {
        QDBusInterface adapter(QStringLiteral("org.bluez"), QStringLiteral("/org/bluez/hci0"),
                               QStringLiteral("org.bluez.Adapter1"), QDBusConnection::systemBus());
        adapter.call(QStringLiteral("StartDiscovery"));
        return;
    }
#endif
    m_devices->upsert({QStringLiteral("11:22:33:44:55:66"), QStringLiteral("New Pixel"), false, false, true});
}

void BluezController::stopDiscovery()
{
    m_discovering = false;
    emit discoveringChanged();
#ifdef EVO_HMI_HAS_DBUS
    if (!m_simulated) {
        QDBusInterface adapter(QStringLiteral("org.bluez"), QStringLiteral("/org/bluez/hci0"),
                               QStringLiteral("org.bluez.Adapter1"), QDBusConnection::systemBus());
        adapter.call(QStringLiteral("StopDiscovery"));
    }
#endif
}

void BluezController::pair(const QString &address)
{
    callDevice(address, QStringLiteral("Pair"));
    if (m_simulated) {
        if (auto *d = m_devices->find(address)) {
            BtDevice copy = *d;
            copy.paired = true;
            m_devices->upsert(copy);
        }
    }
}

void BluezController::connectDevice(const QString &address)
{
    callDevice(address, QStringLiteral("Connect"));
    if (m_simulated) {
        for (const auto &d : m_devices->devices()) {
            BtDevice copy = d;
            copy.connected = (d.address == address);
            m_devices->upsert(copy);
            if (copy.connected)
                m_connectedDeviceName = copy.name;
        }
        emit playerChanged();
    }
}

void BluezController::disconnectDevice(const QString &address)
{
    callDevice(address, QStringLiteral("Disconnect"));
    if (m_simulated) {
        if (auto *d = m_devices->find(address)) {
            BtDevice copy = *d;
            copy.connected = false;
            m_devices->upsert(copy);
        }
        m_connectedDeviceName.clear();
        m_playerStatus = QStringLiteral("paused");
        emit playerChanged();
    }
}

void BluezController::play()
{
    callPlayer(QStringLiteral("Play"));
    m_playerStatus = QStringLiteral("playing");
    emit playerChanged();
}

void BluezController::pause()
{
    callPlayer(QStringLiteral("Pause"));
    m_playerStatus = QStringLiteral("paused");
    emit playerChanged();
}

void BluezController::next()
{
    callPlayer(QStringLiteral("Next"));
    if (m_simulated) {
        m_simTrack = (m_simTrack + 1) % kSimTracksTitle.size();
        m_trackTitle = kSimTracksTitle.at(m_simTrack);
        m_trackArtist = kSimTracksArtist.at(m_simTrack);
        m_positionMs = 0;
        emit playerChanged();
    }
}

void BluezController::previous()
{
    callPlayer(QStringLiteral("Previous"));
    if (m_simulated) {
        m_simTrack = (m_simTrack + kSimTracksTitle.size() - 1) % kSimTracksTitle.size();
        m_trackTitle = kSimTracksTitle.at(m_simTrack);
        m_trackArtist = kSimTracksArtist.at(m_simTrack);
        m_positionMs = 0;
        emit playerChanged();
    }
}

void BluezController::callDevice(const QString &address, const QString &method)
{
#ifdef EVO_HMI_HAS_DBUS
    if (m_simulated)
        return;
    const QString path = QStringLiteral("/org/bluez/hci0/dev_") + QString(address).replace(':', '_');
    QDBusInterface dev(QStringLiteral("org.bluez"), path, QStringLiteral("org.bluez.Device1"),
                       QDBusConnection::systemBus());
    dev.call(method);
#else
    Q_UNUSED(address);
    Q_UNUSED(method);
#endif
}

void BluezController::callPlayer(const QString &method)
{
#ifdef EVO_HMI_HAS_DBUS
    if (m_simulated)
        return;
    const QString path = m_playerPath.isEmpty() ? QStringLiteral("/org/bluez/hci0/dev_00_00_00_00_00_00/player0")
                                                : m_playerPath;
    QDBusInterface player(QStringLiteral("org.bluez"), path, QStringLiteral("org.bluez.MediaPlayer1"),
                          QDBusConnection::systemBus());
    player.call(method);
#else
    Q_UNUSED(method);
#endif
}
