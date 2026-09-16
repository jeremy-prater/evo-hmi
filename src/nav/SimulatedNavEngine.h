#pragma once

#include "nav/NavEngine.h"

class SimulatedNavEngine : public NavEngine
{
    Q_OBJECT

public:
    using NavEngine::NavEngine;
};
