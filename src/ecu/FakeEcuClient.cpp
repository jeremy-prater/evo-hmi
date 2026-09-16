#include "ecu/FakeEcuClient.h"

#include <QtMath>
#include <cmath>

FakeEcuClient::FakeEcuClient(QObject *parent)
    : EcuClient(parent)
{
    setSource(QStringLiteral("fake"));
    setConnected(true);
    m_timer.setInterval(50); // 20 Hz, matching evo-ecu-rs default log period
    connect(&m_timer, &QTimer::timeout, this, &FakeEcuClient::tick);
    m_timer.start();
    tick();
}

void FakeEcuClient::tick()
{
    m_t += 0.05;
    // ~22 s drive cycle: idle, spool, WOT, lift, cruise.
    const double cycle = std::fmod(m_t, 22.0);
    double tps = 8.0;
    double rpm = 850.0;
    double boost = -8.0; // vacuum at idle, PSI gauge-ish
    if (cycle < 4.0) {
        tps = 8.0;
        rpm = 850.0 + 40.0 * std::sin(m_t * 8.0);
        boost = -7.5;
    } else if (cycle < 8.0) {
        const double u = (cycle - 4.0) / 4.0;
        tps = 8.0 + 72.0 * u;
        rpm = 850.0 + 4700.0 * u;
        boost = -7.5 + 26.0 * u;
    } else if (cycle < 12.0) {
        tps = 95.0 + 4.0 * std::sin(m_t * 3.0);
        rpm = 5600.0 + 400.0 * std::sin(m_t * 4.0);
        boost = 18.5 + 1.2 * std::sin(m_t * 5.0);
    } else if (cycle < 15.0) {
        const double u = (cycle - 12.0) / 3.0;
        tps = 95.0 - 80.0 * u;
        rpm = 5800.0 - 2800.0 * u;
        boost = 18.0 - 22.0 * u;
    } else {
        tps = 22.0 + 6.0 * std::sin(m_t * 1.4);
        rpm = 2800.0 + 350.0 * std::sin(m_t * 1.6);
        boost = 3.5 + 1.5 * std::sin(m_t * 1.8);
    }

    rpm = qBound(750.0, rpm, 7500.0);
    const double load = qBound(10.0, tps * 0.85 + qMax(0.0, boost) * 1.4, 100.0);
    const double timing = qBound(2.0, 18.0 - qMax(0.0, boost) * 0.45 + (100.0 - tps) * 0.04, 28.0);
    const double afr = boost > 8.0 ? 11.2 + 0.3 * std::sin(m_t * 6.0)
                                   : 14.4 + 0.4 * std::sin(m_t * 2.0);
    const double coolant = 168.0 + 18.0 * (0.5 + 0.5 * std::sin(m_t * 0.05));
    const double knock = boost > 16.0 ? 0.4 + 0.2 * std::abs(std::sin(m_t * 11.0)) : 0.08;
    const double wgdc = qBound(0.0, qMax(0.0, boost) * 4.2, 85.0);
    const double speed = qBound(0.0, (rpm - 800.0) * 0.018 + qMax(0.0, tps - 10.0) * 0.35, 110.0);
    const double battery = 13.8 + 0.15 * std::sin(m_t * 0.3);

    setSamples(rpm, boost, timing, afr, coolant, knock, wgdc, speed, tps, battery, load);
}
