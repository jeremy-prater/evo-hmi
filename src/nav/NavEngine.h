#pragma once

#include <QObject>
#include <QPair>
#include <QString>
#include <QVariantList>
#include <QVector>
#include <QtQml/qqml.h>

#include "ManeuverModel.h"

class GpsdClient;
class QAbstractListModel;

// Shared nav API for QML. SimulatedNavEngine always works; OsmScoutNavEngine
// subclasses this when libosmscout is linked and a map database is present.
class NavEngine : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    Q_PROPERTY(bool hasVectorMap READ hasVectorMap NOTIFY backendChanged)
    Q_PROPERTY(QString backend READ backend NOTIFY backendChanged)
    Q_PROPERTY(double latitude READ latitude NOTIFY positionChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY positionChanged)
    Q_PROPERTY(double headingDeg READ headingDeg NOTIFY positionChanged)
    Q_PROPERTY(double speedMps READ speedMps NOTIFY positionChanged)
    Q_PROPERTY(int fixMode READ fixMode NOTIFY positionChanged)
    Q_PROPERTY(QString gpsStatus READ gpsStatus NOTIFY gpsStatusChanged)
    Q_PROPERTY(bool navigating READ navigating NOTIFY routeChanged)
    Q_PROPERTY(bool routing READ routing NOTIFY routingChanged)
    Q_PROPERTY(QString destinationName READ destinationName NOTIFY routeChanged)
    Q_PROPERTY(double distanceRemainingM READ distanceRemainingM NOTIFY guidanceChanged)
    Q_PROPERTY(double etaSeconds READ etaSeconds NOTIFY guidanceChanged)
    Q_PROPERTY(QString nextInstruction READ nextInstruction NOTIFY guidanceChanged)
    Q_PROPERTY(QString nextManeuverType READ nextManeuverType NOTIFY guidanceChanged)
    Q_PROPERTY(double distanceToNextM READ distanceToNextM NOTIFY guidanceChanged)
    Q_PROPERTY(QVariantList routePoints READ routePoints NOTIFY routeChanged)
    Q_PROPERTY(QVariantList searchResults READ searchResults NOTIFY searchChanged)
    Q_PROPERTY(ManeuverModel *maneuvers READ maneuvers CONSTANT)
    Q_PROPERTY(double mapCenterLat READ mapCenterLat NOTIFY mapViewChanged)
    Q_PROPERTY(double mapCenterLon READ mapCenterLon NOTIFY mapViewChanged)
    Q_PROPERTY(double mapZoom READ mapZoom WRITE setMapZoom NOTIFY mapViewChanged)
    Q_PROPERTY(bool followVehicle READ followVehicle WRITE setFollowVehicle NOTIFY mapViewChanged)

public:
    explicit NavEngine(GpsdClient *gpsd, QObject *parent = nullptr);

    bool hasVectorMap() const { return m_hasVectorMap; }
    QString backend() const { return m_backend; }
    double latitude() const { return m_latitude; }
    double longitude() const { return m_longitude; }
    double headingDeg() const { return m_headingDeg; }
    double speedMps() const { return m_speedMps; }
    int fixMode() const { return m_fixMode; }
    QString gpsStatus() const { return m_gpsStatus; }
    bool navigating() const { return m_navigating; }
    bool routing() const { return m_routing; }
    QString destinationName() const { return m_destinationName; }
    double distanceRemainingM() const { return m_distanceRemainingM; }
    double etaSeconds() const { return m_etaSeconds; }
    QString nextInstruction() const { return m_nextInstruction; }
    QString nextManeuverType() const { return m_nextManeuverType; }
    double distanceToNextM() const { return m_distanceToNextM; }
    QVariantList routePoints() const { return m_routePoints; }
    QVariantList searchResults() const { return m_searchResults; }
    ManeuverModel *maneuvers() const { return m_maneuvers; }
    double mapCenterLat() const { return m_mapCenterLat; }
    double mapCenterLon() const { return m_mapCenterLon; }
    double mapZoom() const { return m_mapZoom; }
    bool followVehicle() const { return m_followVehicle; }

    GpsdClient *gpsd() const { return m_gpsd; }

    Q_INVOKABLE virtual void search(const QString &query);
    Q_INVOKABLE virtual void navigateTo(double lat, double lon, const QString &name);
    Q_INVOKABLE virtual void cancelRoute();
    Q_INVOKABLE void recenter();
    void setMapZoom(double zoom);
    void setFollowVehicle(bool follow);
    Q_INVOKABLE void panTo(double lat, double lon);

signals:
    void backendChanged();
    void positionChanged();
    void gpsStatusChanged();
    void routeChanged();
    void routingChanged();
    void guidanceChanged();
    void searchChanged();
    void mapViewChanged();

protected:
    void onGps();
    void updateGuidance();
    void buildSimulatedRoute(double lat, double lon, const QString &name);

    GpsdClient *m_gpsd = nullptr;
    ManeuverModel *m_maneuvers = nullptr;
    bool m_hasVectorMap = false;
    QString m_backend = QStringLiteral("simulated");
    double m_latitude = 0;
    double m_longitude = 0;
    double m_headingDeg = 0;
    double m_speedMps = 0;
    int m_fixMode = 0;
    QString m_gpsStatus;
    bool m_navigating = false;
    bool m_routing = false;
    QString m_destinationName;
    double m_distanceRemainingM = 0;
    double m_etaSeconds = 0;
    QString m_nextInstruction;
    QString m_nextManeuverType;
    double m_distanceToNextM = 0;
    QVariantList m_routePoints;
    QVariantList m_searchResults;
    double m_mapCenterLat = 0;
    double m_mapCenterLon = 0;
    double m_mapZoom = 16.0;
    bool m_followVehicle = true;
    int m_maneuverIndex = 0;
    QVector<QPair<double, double>> m_routeLatLon;
};
