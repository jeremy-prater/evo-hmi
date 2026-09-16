#pragma once

#include "ecu/EcuClient.h"

#include <QTimer>

class FakeEcuClient : public EcuClient
{
    Q_OBJECT

public:
    explicit FakeEcuClient(QObject *parent = nullptr);

private:
    void tick();

    QTimer m_timer;
    double m_t = 0;
};
