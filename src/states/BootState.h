#pragma once
#include "states/AppContext.h"
#include "states/AppState.h"

class BootState
{
   public:
    AppState tick(AppContext& ctx);
};
