#pragma once
#include <M5EPD.h>
#include "AppConfig.h"
#include "setup/SetupEntryReason.h"
#include "storage/config/NvsConfigProvider.h"

struct AppContext
{
    AppContext(M5EPD_Canvas& canvasRef, NvsConfigProvider& nvsRef, AppConfig& appConfigRef)
        : canvas(canvasRef), nvs(nvsRef), appConfig(appConfigRef)
    {
    }

    M5EPD_Canvas& canvas;
    NvsConfigProvider& nvs;
    AppConfig& appConfig;
    SetupEntryReason setupEntryReason = SetupEntryReason::NoConfig;
};
