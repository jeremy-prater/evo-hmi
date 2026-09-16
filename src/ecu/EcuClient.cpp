#include "ecu/EcuClient.h"

EcuClient::EcuClient(QObject *parent)
    : QObject(parent)
{
}

void EcuClient::setConnected(bool connected)
{
    if (m_connected == connected)
        return;
    m_connected = connected;
    emit connectedChanged();
}

void EcuClient::setSource(const QString &source)
{
    if (m_source == source)
        return;
    m_source = source;
    emit sourceChanged();
}

void EcuClient::setSamples(double rpm, double boostPsi, double timingDeg, double afr,
                           double coolantF, double knock, double wgdc, double speedMph,
                           double tps, double battery, double load)
{
    m_rpm = rpm;
    m_boostPsi = boostPsi;
    m_timingDeg = timingDeg;
    m_afr = afr;
    m_coolantF = coolantF;
    m_knock = knock;
    m_wgdc = wgdc;
    m_speedMph = speedMph;
    m_tps = tps;
    m_battery = battery;
    m_load = load;
    emit samplesChanged();
}
