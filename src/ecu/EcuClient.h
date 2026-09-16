#pragma once

#include <QObject>
#include <QString>

// Live ECU samples for the engine screen.
// A real protobuf Unix-socket client will replace FakeEcuClient later
// (u32 LE length + Envelope on /tmp/evo-ecu.sock or /run/evo-ecu.sock).
class EcuClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString source READ source NOTIFY sourceChanged)
    Q_PROPERTY(double rpm READ rpm NOTIFY samplesChanged)
    Q_PROPERTY(double boostPsi READ boostPsi NOTIFY samplesChanged)
    Q_PROPERTY(double timingDeg READ timingDeg NOTIFY samplesChanged)
    Q_PROPERTY(double afr READ afr NOTIFY samplesChanged)
    Q_PROPERTY(double coolantF READ coolantF NOTIFY samplesChanged)
    Q_PROPERTY(double knock READ knock NOTIFY samplesChanged)
    Q_PROPERTY(double wgdc READ wgdc NOTIFY samplesChanged)
    Q_PROPERTY(double speedMph READ speedMph NOTIFY samplesChanged)
    Q_PROPERTY(double tps READ tps NOTIFY samplesChanged)
    Q_PROPERTY(double battery READ battery NOTIFY samplesChanged)
    Q_PROPERTY(double load READ load NOTIFY samplesChanged)

public:
    explicit EcuClient(QObject *parent = nullptr);

    bool connected() const { return m_connected; }
    QString source() const { return m_source; }
    double rpm() const { return m_rpm; }
    double boostPsi() const { return m_boostPsi; }
    double timingDeg() const { return m_timingDeg; }
    double afr() const { return m_afr; }
    double coolantF() const { return m_coolantF; }
    double knock() const { return m_knock; }
    double wgdc() const { return m_wgdc; }
    double speedMph() const { return m_speedMph; }
    double tps() const { return m_tps; }
    double battery() const { return m_battery; }
    double load() const { return m_load; }

signals:
    void connectedChanged();
    void sourceChanged();
    void samplesChanged();

protected:
    void setConnected(bool connected);
    void setSource(const QString &source);
    void setSamples(double rpm, double boostPsi, double timingDeg, double afr,
                    double coolantF, double knock, double wgdc, double speedMph,
                    double tps, double battery, double load);

    bool m_connected = false;
    QString m_source;
    double m_rpm = 0;
    double m_boostPsi = 0;
    double m_timingDeg = 0;
    double m_afr = 14.7;
    double m_coolantF = 0;
    double m_knock = 0;
    double m_wgdc = 0;
    double m_speedMph = 0;
    double m_tps = 0;
    double m_battery = 0;
    double m_load = 0;
};
