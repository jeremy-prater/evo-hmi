#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>
#include <QtQml/qqml.h>

struct BtDevice {
    QString address;
    QString name;
    bool paired = false;
    bool connected = false;
    bool audio = false;
};

class DeviceModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by BluezController")
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        AddressRole = Qt::UserRole + 1,
        NameRole,
        PairedRole,
        ConnectedRole,
        AudioRole
    };

    explicit DeviceModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void upsert(const BtDevice &device);
    void removePath(const QString &address);
    void clear();
    BtDevice *find(const QString &address);
    QVector<BtDevice> devices() const { return m_items; }

signals:
    void countChanged();

private:
    int indexOf(const QString &address) const;
    QVector<BtDevice> m_items;
};
