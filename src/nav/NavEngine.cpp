#include "nav/NavEngine.h"

#include "nav/GpsdClient.h"
#include "nav/ManeuverModel.h"

#include <QtMath>
#include <cmath>

namespace {
double haversineM(double lat1, double lon1, double lat2, double lon2)
{
    constexpr double r = 6371000.0;
    const double p1 = qDegreesToRadians(lat1);
    const double p2 = qDegreesToRadians(lat2);
    const double dp = qDegreesToRadians(lat2 - lat1);
    const double dl = qDegreesToRadians(lon2 - lon1);
    const double a = std::sin(dp / 2) * std::sin(dp / 2)
        + std::cos(p1) * std::cos(p2) * std::sin(dl / 2) * std::sin(dl / 2);
    return 2 * r * std::atan2(std::sqrt(a), std::sqrt(1 - a));
}
}

NavEngine::NavEngine(GpsdClient *gpsd, QObject *parent)
    : QObject(parent)
    , m_gpsd(gpsd)
    , m_maneuvers(new ManeuverModel(this))
{
    if (m_gpsd) {
        connect(m_gpsd, &GpsdClient::positionChanged, this, &NavEngine::onGps);
        connect(m_gpsd, &GpsdClient::statusChanged, this, [this]() {
            m_gpsStatus = m_gpsd->statusText();
            emit gpsStatusChanged();
        });
        onGps();
        m_gpsStatus = m_gpsd->statusText();
    }
}

void NavEngine::onGps()
{
    if (!m_gpsd)
        return;
    m_latitude = m_gpsd->latitude();
    m_longitude = m_gpsd->longitude();
    m_headingDeg = m_gpsd->headingDeg();
    m_speedMps = m_gpsd->speedMps();
    m_fixMode = m_gpsd->fixMode();
    if (m_followVehicle) {
        m_mapCenterLat = m_latitude;
        m_mapCenterLon = m_longitude;
        emit mapViewChanged();
    }
    emit positionChanged();
    if (m_navigating)
        updateGuidance();
}

void NavEngine::search(const QString &query)
{
    m_searchResults.clear();
    const QString q = query.trimmed();
    if (q.isEmpty()) {
        emit searchChanged();
        return;
    }
    // Stand-in POIs around the current fix until libosmscout geocoding is enabled.
    const QVariantList offsets = {
        QVariantMap{{"name", q + QStringLiteral(" (ahead)")}, {"lat", m_latitude + 0.012}, {"lon", m_longitude + 0.008}},
        QVariantMap{{"name", q + QStringLiteral(" (east)")}, {"lat", m_latitude + 0.004}, {"lon", m_longitude + 0.02}},
        QVariantMap{{"name", q + QStringLiteral(" (south)")}, {"lat", m_latitude - 0.015}, {"lon", m_longitude + 0.004}},
    };
    m_searchResults = offsets;
    emit searchChanged();
}

void NavEngine::navigateTo(double lat, double lon, const QString &name)
{
    buildSimulatedRoute(lat, lon, name);
}

void NavEngine::cancelRoute()
{
    m_navigating = false;
    m_routing = false;
    m_destinationName.clear();
    m_routePoints.clear();
    m_routeLatLon.clear();
    m_nextInstruction.clear();
    m_nextManeuverType.clear();
    m_distanceRemainingM = 0;
    m_distanceToNextM = 0;
    m_etaSeconds = 0;
    m_maneuverIndex = 0;
    m_maneuvers->clear();
    emit routeChanged();
    emit routingChanged();
    emit guidanceChanged();
}

void NavEngine::recenter()
{
    m_followVehicle = true;
    m_mapCenterLat = m_latitude;
    m_mapCenterLon = m_longitude;
    emit mapViewChanged();
}

void NavEngine::setMapZoom(double zoom)
{
    zoom = qBound(10.0, zoom, 19.0);
    if (qFuzzyCompare(m_mapZoom, zoom))
        return;
    m_mapZoom = zoom;
    emit mapViewChanged();
}

void NavEngine::setFollowVehicle(bool follow)
{
    if (m_followVehicle == follow)
        return;
    m_followVehicle = follow;
    if (follow) {
        m_mapCenterLat = m_latitude;
        m_mapCenterLon = m_longitude;
    }
    emit mapViewChanged();
}

void NavEngine::panTo(double lat, double lon)
{
    m_followVehicle = false;
    m_mapCenterLat = lat;
    m_mapCenterLon = lon;
    emit mapViewChanged();
}

void NavEngine::buildSimulatedRoute(double lat, double lon, const QString &name)
{
    m_routing = true;
    emit routingChanged();

    m_destinationName = name.isEmpty() ? QStringLiteral("Destination") : name;
    m_routeLatLon.clear();
    m_routePoints.clear();

    const int steps = 24;
    for (int i = 0; i <= steps; ++i) {
        const double u = double(i) / double(steps);
        const double bend = std::sin(u * M_PI) * 0.004;
        const double rlat = m_latitude + (lat - m_latitude) * u + bend;
        const double rlon = m_longitude + (lon - m_longitude) * u;
        m_routeLatLon.append({rlat, rlon});
        m_routePoints.append(QVariantMap{{"lat", rlat}, {"lon", rlon}});
    }

    const double total = haversineM(m_latitude, m_longitude, lat, lon);
    QVector<Maneuver> moves;
    moves.append({QStringLiteral("depart"), QStringLiteral("Depart toward %1").arg(m_destinationName), 0});
    moves.append({QStringLiteral("right"), QStringLiteral("Turn right"), total * 0.28});
    moves.append({QStringLiteral("continue"), QStringLiteral("Continue straight"), total * 0.55});
    moves.append({QStringLiteral("left"), QStringLiteral("Turn left"), total * 0.82});
    moves.append({QStringLiteral("arrive"), QStringLiteral("Arrive at %1").arg(m_destinationName), total});
    m_maneuvers->setManeuvers(moves);
    m_maneuverIndex = 0;
    m_navigating = true;
    m_routing = false;
    emit routingChanged();
    emit routeChanged();
    updateGuidance();
}

void NavEngine::updateGuidance()
{
    if (!m_navigating || m_routeLatLon.isEmpty())
        return;
    const auto dest = m_routeLatLon.last();
    m_distanceRemainingM = haversineM(m_latitude, m_longitude, dest.first, dest.second);
    const double speed = qMax(m_speedMps, 8.0);
    m_etaSeconds = m_distanceRemainingM / speed;

    const auto &moves = m_maneuvers->maneuvers();
    if (!moves.isEmpty()) {
        while (m_maneuverIndex + 1 < moves.size()
               && m_distanceRemainingM < (moves.last().distanceMeters - moves.at(m_maneuverIndex).distanceMeters) * 0.15) {
            ++m_maneuverIndex;
        }
        const int idx = qMin(m_maneuverIndex, moves.size() - 1);
        m_nextInstruction = moves.at(idx).instruction;
        m_nextManeuverType = moves.at(idx).type;
        const double destM = moves.last().distanceMeters;
        m_distanceToNextM = qMax(0.0, (moves.at(idx).distanceMeters) - (destM - m_distanceRemainingM));
        if (m_nextManeuverType == QLatin1String("arrive"))
            m_distanceToNextM = m_distanceRemainingM;
    }
    emit guidanceChanged();
}
