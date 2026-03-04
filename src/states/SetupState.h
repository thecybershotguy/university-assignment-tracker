#pragma once
#include "setup/SetupMode.h"
#include "states/AppContext.h"
#include "states/AppState.h"

class SetupState
{
   public:
    SetupState(M5EPD_Canvas& canvas, NvsConfigProvider& nvs);

    AppState tick(AppContext& ctx);

   private:
    SetupMode setupMode_;
};
