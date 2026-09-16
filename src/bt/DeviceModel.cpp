#include "bt/DeviceModel.h"

DeviceModel::DeviceModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
        return {};
    const auto &d = m_items.at(index.row());
    switch (role) {
    case AddressRole:
        return d.address;
    case NameRole:
        return d.name;
    case PairedRole:
        return d.paired;
    case ConnectedRole:
        return d.connected;
    case AudioRole:
        return d.audio;
    default:
        return {};
    }
}

QHash<int, QByteArray> DeviceModel::roleNames() const
{
    return {
        {AddressRole, "address"},
        {NameRole, "name"},
        {PairedRole, "paired"},
        {ConnectedRole, "connected"},
        {AudioRole, "audio"},
    };
}

int DeviceModel::indexOf(const QString &address) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items.at(i).address == address)
            return i;
    }
    return -1;
}

BtDevice *DeviceModel::find(const QString &address)
{
    const int i = indexOf(address);
    return i < 0 ? nullptr : &m_items[i];
}

void DeviceModel::upsert(const BtDevice &device)
{
    const int i = indexOf(device.address);
    if (i < 0) {
        beginInsertRows({}, m_items.size(), m_items.size());
        m_items.append(device);
        endInsertRows();
        emit countChanged();
        return;
    }
    m_items[i] = device;
    const auto idx = index(i, 0);
    emit dataChanged(idx, idx);
}

void DeviceModel::removePath(const QString &address)
{
    const int i = indexOf(address);
    if (i < 0)
        return;
    beginRemoveRows({}, i, i);
    m_items.removeAt(i);
    endRemoveRows();
    emit countChanged();
}

void DeviceModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
    emit countChanged();
}
