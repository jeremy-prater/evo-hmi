#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>
#include <QtQml/qqml.h>

struct Maneuver {
    QString type;
    QString instruction;
    double distanceMeters = 0;
};

class ManeuverModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by NavEngine")
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles { TypeRole = Qt::UserRole + 1, InstructionRole, DistanceRole };

    explicit ManeuverModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setManeuvers(QVector<Maneuver> maneuvers);
    void clear();
    const QVector<Maneuver> &maneuvers() const { return m_items; }

signals:
    void countChanged();

private:
    QVector<Maneuver> m_items;
};
