#include "states/BootState.h"
#include "storage/config/CompositeConfigProvider.h"

AppState BootState::tick(AppContext& ctx)
{
#if APP_DEV_MODE
    CompositeConfigProvider cfgProvider;
    cfgProvider.seedFromHardcodedIfEmpty();
    if (!cfgProvider.load(ctx.appConfig))
    {
        ctx.setupEntryReason = SetupEntryReason::NoConfig;
        return AppState::Setup;
    }
#else
    if (!ctx.nvs.hasConfig() || !ctx.nvs.load(ctx.appConfig))
    {
        ctx.setupEntryReason = SetupEntryReason::NoConfig;
        return AppState::Setup;
    }
#endif

    Serial.printf("WiFi SSID in use: %s\n", ctx.appConfig.wifiSsid.c_str());
    Serial.printf("Course count: %d\n", (int)ctx.appConfig.courses.size());
    Serial.printf("Timezone offset: %ld\n", ctx.appConfig.timezoneOffsetSec);

    return AppState::Runtime;
}
