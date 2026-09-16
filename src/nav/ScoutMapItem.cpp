#include "nav/ScoutMapItem.h"

#include "nav/NavEngine.h"

#include <QPainter>
#include <cmath>

#ifdef EVO_HMI_HAS_OSMSCOUT
#include "nav/OsmScoutNavEngine.h"
#include <osmscout/projection/MercatorProjection.h>
#include <osmscoutmap/MapParameter.h>
#include <osmscoutmap/MapData.h>
#include <osmscoutmapqt/MapPainterQt.h>
#include <memory>
#endif

ScoutMapItem::ScoutMapItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setAntialiasing(true);
}

void ScoutMapItem::setEngine(NavEngine *engine)
{
    if (m_engine == engine)
        return;
    if (m_engine)
        disconnect(m_engine, nullptr, this, nullptr);
    m_engine = engine;
    bindEngine();
    emit engineChanged();
    update();
}

void ScoutMapItem::bindEngine()
{
    const bool wasReady = m_ready;
    m_ready = false;
    if (m_engine) {
        connect(m_engine, &NavEngine::positionChanged, this, qOverload<>(&QQuickItem::update));
        connect(m_engine, &NavEngine::mapViewChanged, this, qOverload<>(&QQuickItem::update));
        connect(m_engine, &NavEngine::routeChanged, this, qOverload<>(&QQuickItem::update));
#ifdef EVO_HMI_HAS_OSMSCOUT
        if (auto *scout = qobject_cast<OsmScoutNavEngine *>(m_engine))
            m_ready = scout->databaseOpen();
#endif
    }
    if (wasReady != m_ready)
        emit readyChanged();
}

void ScoutMapItem::paint(QPainter *painter)
{
    if (!painter)
        return;
    painter->fillRect(boundingRect(), QColor(QStringLiteral("#12161e")));

#ifdef EVO_HMI_HAS_OSMSCOUT
    auto *scout = qobject_cast<OsmScoutNavEngine *>(m_engine);
    if (!scout || !scout->databaseOpen() || width() < 8 || height() < 8)
        return;

    using MapServicePtr = std::shared_ptr<osmscout::MapService>;
    using StylePtr = std::shared_ptr<osmscout::StyleConfig>;
    auto mapService = *static_cast<MapServicePtr *>(scout->mapServiceHandle());
    auto style = *static_cast<StylePtr *>(scout->styleHandle());
    if (!mapService || !style)
        return;

    osmscout::MercatorProjection projection;
    const osmscout::GeoCoord center(m_engine->mapCenterLat(), m_engine->mapCenterLon());
    const double magnification = std::pow(2.0, m_engine->mapZoom());
    projection.Set(center,
                   osmscout::Bearing::Degrees(m_engine->followVehicle() ? m_engine->headingDeg() : 0.0),
                   osmscout::Magnification(magnification),
                   96.0,
                   int(width()),
                   int(height()));

    osmscout::AreaSearchParameter searchParam;
    osmscout::MapData data;
    mapService->GetObjects(searchParam, *style, projection, data);

    osmscout::MapParameter mapParam;
    osmscout::MapPainterQt mapPainter(style);
    mapPainter.DrawMap(projection, mapParam, data, painter);
#else
    Q_UNUSED(painter);
#endif
}
