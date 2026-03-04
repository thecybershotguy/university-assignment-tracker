#pragma once
#include "states/AppContext.h"
#include "states/AppState.h"

class SleepState
{
   public:
    AppState tick(AppContext& ctx);
};
