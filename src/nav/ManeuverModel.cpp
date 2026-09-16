#include "nav/ManeuverModel.h"

#include <utility>

ManeuverModel::ManeuverModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ManeuverModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

QVariant ManeuverModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
        return {};
    const auto &item = m_items.at(index.row());
    switch (role) {
    case TypeRole:
        return item.type;
    case InstructionRole:
        return item.instruction;
    case DistanceRole:
        return item.distanceMeters;
    default:
        return {};
    }
}

QHash<int, QByteArray> ManeuverModel::roleNames() const
{
    return {{TypeRole, "type"}, {InstructionRole, "instruction"}, {DistanceRole, "distanceMeters"}};
}

void ManeuverModel::setManeuvers(QVector<Maneuver> maneuvers)
{
    beginResetModel();
    m_items = std::move(maneuvers);
    endResetModel();
    emit countChanged();
}

void ManeuverModel::clear()
{
    setManeuvers({});
}
