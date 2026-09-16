#pragma once

#include "NavEngine.h"

#include <QQuickPaintedItem>
#include <QString>
#include <QtQml/qqml.h>

// Vector map surface. With libosmscout this paints OSM tiles from the
// local database; otherwise it is unused (QML falls back to NavMap.qml).
class ScoutMapItem : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(NavEngine *engine READ engine WRITE setEngine NOTIFY engineChanged)
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)

public:
    explicit ScoutMapItem(QQuickItem *parent = nullptr);

    NavEngine *engine() const { return m_engine; }
    void setEngine(NavEngine *engine);
    bool ready() const { return m_ready; }

    void paint(QPainter *painter) override;

signals:
    void engineChanged();
    void readyChanged();

private:
    void bindEngine();

    NavEngine *m_engine = nullptr;
    bool m_ready = false;
};
