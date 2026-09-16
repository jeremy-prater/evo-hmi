#include "nav/OsmScoutNavEngine.h"

#ifdef EVO_HMI_HAS_OSMSCOUT

#include "nav/GpsdClient.h"
#include "nav/ManeuverModel.h"

#include <osmscout/db/Database.h>
#include <osmscout/location/LocationService.h>
#include <osmscout/routing/SimpleRoutingService.h>
#include <osmscout/routing/FastestPathRoutingProfile.h>
#include <osmscoutmap/MapService.h>
#include <osmscoutmap/StyleConfig.h>

#include <QDir>
#include <QFileInfo>
#include <memory>

using DatabasePtr = std::shared_ptr<osmscout::Database>;
using MapServicePtr = std::shared_ptr<osmscout::MapService>;
using StylePtr = std::shared_ptr<osmscout::StyleConfig>;

OsmScoutNavEngine::OsmScoutNavEngine(GpsdClient *gpsd, const QString &mapDir, QObject *parent)
    : NavEngine(gpsd, parent)
    , m_mapDir(mapDir)
{
    if (m_mapDir.isEmpty())
        m_mapDir = qEnvironmentVariable("EVO_HMI_MAP_DIR");
    if (m_mapDir.isEmpty())
        m_mapDir = QDir::homePath() + QStringLiteral("/.local/share/evo-hmi/maps");

    if (openDatabase(m_mapDir)) {
        m_hasVectorMap = true;
        m_backend = QStringLiteral("libosmscout");
        emit backendChanged();
    } else {
        m_backend = QStringLiteral("simulated (no osmscout db)");
        emit backendChanged();
    }
}

OsmScoutNavEngine::~OsmScoutNavEngine()
{
    delete static_cast<DatabasePtr *>(m_database);
    delete static_cast<MapServicePtr *>(m_mapService);
    delete static_cast<StylePtr *>(m_style);
}

bool OsmScoutNavEngine::databaseOpen() const
{
    return m_database != nullptr;
}

bool OsmScoutNavEngine::openDatabase(const QString &mapDir)
{
    osmscout::DatabaseParameter param;
    auto database = std::make_shared<osmscout::Database>(param);
    if (!database->Open(mapDir.toStdString()))
        return false;

    auto mapService = std::make_shared<osmscout::MapService>(database);

    QString stylePath = mapDir + QStringLiteral("/standard.oss");
    if (!QFileInfo::exists(stylePath))
        stylePath = mapDir + QStringLiteral("/stylesheets/standard.oss");

    auto style = std::make_shared<osmscout::StyleConfig>(database->GetTypeConfig());
    if (QFileInfo::exists(stylePath) && !style->Load(stylePath.toStdString()))
        return false;

    m_database = new DatabasePtr(std::move(database));
    m_mapService = new MapServicePtr(std::move(mapService));
    m_style = new StylePtr(std::move(style));
    return true;
}

void OsmScoutNavEngine::search(const QString &query)
{
    if (!m_database) {
        NavEngine::search(query);
        return;
    }
    auto database = *static_cast<DatabasePtr *>(m_database);
    osmscout::LocationService locations(database);
    osmscout::LocationSearch search;
    search.limit = 8;
    osmscout::LocationSearchResult result;
    if (!locations.SearchForLocationsByString(query.toStdString(), search, result)) {
        NavEngine::search(query);
        return;
    }
    m_searchResults.clear();
    for (const auto &entry : result.results) {
        osmscout::GeoCoord coord;
        QString name = QString::fromStdString(entry.adminRegion ? entry.adminRegion->name : query.toStdString());
        if (entry.location)
            name = QString::fromStdString(entry.location->name);
        if (entry.object.GetCoord(coord) || true) {
            // Prefer location coordinate when present.
            if (entry.location && !entry.location->nodes.empty())
                coord = entry.location->nodes.front();
            else if (entry.adminRegion)
                coord = entry.adminRegion->center;
            m_searchResults.append(QVariantMap{
                {QStringLiteral("name"), name},
                {QStringLiteral("lat"), coord.GetLat()},
                {QStringLiteral("lon"), coord.GetLon()},
            });
        }
    }
    if (m_searchResults.isEmpty())
        NavEngine::search(query);
    else
        emit searchChanged();
}

void OsmScoutNavEngine::navigateTo(double lat, double lon, const QString &name)
{
    if (!m_database) {
        NavEngine::navigateTo(lat, lon, name);
        return;
    }
    auto database = *static_cast<DatabasePtr *>(m_database);

    m_routing = true;
    emit routingChanged();

    osmscout::RouterParameter routerParam;
    osmscout::SimpleRoutingService router(database, routerParam, "/car");
    auto profile = std::make_shared<osmscout::FastestPathRoutingProfile>(database->GetTypeConfig());

    osmscout::RoutePosition start;
    osmscout::RoutePosition target;
    const osmscout::GeoCoord from(m_latitude, m_longitude);
    const osmscout::GeoCoord to(lat, lon);

    if (!router.TransformCoordinateToRoutePosition(from, *profile, start)
        || !router.TransformCoordinateToRoutePosition(to, *profile, target)) {
        m_routing = false;
        emit routingChanged();
        NavEngine::navigateTo(lat, lon, name);
        return;
    }

    osmscout::RoutingResult route = router.CalculateRoute(*profile, start, target, osmscout::RoutingParameter());
    if (!route.Success()) {
        m_routing = false;
        emit routingChanged();
        NavEngine::navigateTo(lat, lon, name);
        return;
    }

    m_destinationName = name.isEmpty() ? QStringLiteral("Destination") : name;
    m_routeLatLon.clear();
    m_routePoints.clear();
    const auto points = router.TransformRouteDataToPoints(route.GetRoute());
    for (const auto &p : points.GetPoints()) {
        m_routeLatLon.append({p.GetLat(), p.GetLon()});
        m_routePoints.append(QVariantMap{{"lat", p.GetLat()}, {"lon", p.GetLon()}});
    }

    osmscout::RouteDescription desc;
    router.TransformRouteDataToRouteDescription(route.GetRoute(), desc);
    QVector<Maneuver> moves;
    double acc = 0;
    for (auto node = desc.Nodes().begin(); node != desc.Nodes().end(); ++node) {
        acc = node->GetDistance().AsMeter();
        for (const auto &descObj : node->GetDescriptions()) {
            auto turn = dynamic_cast<const osmscout::RouteDescription::TurnDescription *>(descObj.get());
            if (turn) {
                QString type = QStringLiteral("continue");
                const auto dir = turn->GetTurn();
                if (dir == osmscout::RouteDescription::left || dir == osmscout::RouteDescription::slightlyLeft)
                    type = QStringLiteral("left");
                else if (dir == osmscout::RouteDescription::right || dir == osmscout::RouteDescription::slightlyRight)
                    type = QStringLiteral("right");
                moves.append({type, QString::fromStdString(turn->GetDisplayName()), acc});
            }
        }
    }
    if (moves.isEmpty())
        moves.append({QStringLiteral("arrive"), QStringLiteral("Arrive at %1").arg(m_destinationName), acc});
    else
        moves.append({QStringLiteral("arrive"), QStringLiteral("Arrive at %1").arg(m_destinationName), acc});

    m_maneuvers->setManeuvers(moves);
    m_maneuverIndex = 0;
    m_navigating = true;
    m_routing = false;
    emit routingChanged();
    emit routeChanged();
    updateGuidance();
}

#endif
