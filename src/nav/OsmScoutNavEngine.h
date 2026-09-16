#pragma once

#include "nav/NavEngine.h"

#ifdef EVO_HMI_HAS_OSMSCOUT

class OsmScoutNavEngine : public NavEngine
{
    Q_OBJECT

public:
    OsmScoutNavEngine(GpsdClient *gpsd, const QString &mapDir, QObject *parent = nullptr);
    ~OsmScoutNavEngine() override;

    void search(const QString &query) override;
    void navigateTo(double lat, double lon, const QString &name) override;

    bool databaseOpen() const;
    QString mapDir() const { return m_mapDir; }

    // Opaque handle for ScoutMapItem (osmscout::Database / MapService).
    void *databaseHandle() const { return m_database; }
    void *mapServiceHandle() const { return m_mapService; }
    void *styleHandle() const { return m_style; }

private:
    bool openDatabase(const QString &mapDir);

    QString m_mapDir;
    void *m_database = nullptr;
    void *m_mapService = nullptr;
    void *m_style = nullptr;
};

#endif
