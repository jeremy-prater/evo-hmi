#pragma once

#include "DeviceModel.h"

#include <QObject>
#include <QString>
#include <QTimer>

class BluezController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool available READ available NOTIFY availableChanged)
    Q_PROPERTY(bool simulated READ simulated NOTIFY availableChanged)
    Q_PROPERTY(bool powered READ powered WRITE setPowered NOTIFY poweredChanged)
    Q_PROPERTY(bool discovering READ discovering NOTIFY discoveringChanged)
    Q_PROPERTY(QString adapterName READ adapterName NOTIFY adapterChanged)
    Q_PROPERTY(QString connectedDeviceName READ connectedDeviceName NOTIFY playerChanged)
    Q_PROPERTY(QString trackTitle READ trackTitle NOTIFY playerChanged)
    Q_PROPERTY(QString trackArtist READ trackArtist NOTIFY playerChanged)
    Q_PROPERTY(QString trackAlbum READ trackAlbum NOTIFY playerChanged)
    Q_PROPERTY(QString playerStatus READ playerStatus NOTIFY playerChanged)
    Q_PROPERTY(qint64 positionMs READ positionMs NOTIFY playerChanged)
    Q_PROPERTY(qint64 durationMs READ durationMs NOTIFY playerChanged)
    Q_PROPERTY(DeviceModel *devices READ devices CONSTANT)

public:
    explicit BluezController(QObject *parent = nullptr);

    bool available() const { return m_available; }
    bool simulated() const { return m_simulated; }
    bool powered() const { return m_powered; }
    bool discovering() const { return m_discovering; }
    QString adapterName() const { return m_adapterName; }
    QString connectedDeviceName() const { return m_connectedDeviceName; }
    QString trackTitle() const { return m_trackTitle; }
    QString trackArtist() const { return m_trackArtist; }
    QString trackAlbum() const { return m_trackAlbum; }
    QString playerStatus() const { return m_playerStatus; }
    qint64 positionMs() const { return m_positionMs; }
    qint64 durationMs() const { return m_durationMs; }
    DeviceModel *devices() const { return m_devices; }

    void setForceSimulated(bool force);
    void start();

    Q_INVOKABLE void setPowered(bool powered);
    Q_INVOKABLE void startDiscovery();
    Q_INVOKABLE void stopDiscovery();
    Q_INVOKABLE void pair(const QString &address);
    Q_INVOKABLE void connectDevice(const QString &address);
    Q_INVOKABLE void disconnectDevice(const QString &address);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void next();
    Q_INVOKABLE void previous();

signals:
    void availableChanged();
    void poweredChanged();
    void discoveringChanged();
    void adapterChanged();
    void playerChanged();

private:
    void startSimulated();
    void simTick();
    bool tryStartBluez();
    void callDevice(const QString &address, const QString &method);
    void callPlayer(const QString &method);

    DeviceModel *m_devices = nullptr;
    QTimer m_simTimer;
    bool m_forceSim = false;
    bool m_available = false;
    bool m_simulated = true;
    bool m_powered = true;
    bool m_discovering = false;
    QString m_adapterName = QStringLiteral("hci0");
    QString m_connectedDeviceName;
    QString m_trackTitle;
    QString m_trackArtist;
    QString m_trackAlbum;
    QString m_playerStatus = QStringLiteral("paused");
    qint64 m_positionMs = 0;
    qint64 m_durationMs = 0;
    QString m_playerPath;
    int m_simTrack = 0;
};
