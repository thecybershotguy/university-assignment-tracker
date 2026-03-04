#pragma once
#include "states/AppContext.h"
#include "states/AppState.h"

class RuntimeState
{
   public:
    AppState tick(AppContext& ctx);
};
