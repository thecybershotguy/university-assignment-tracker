#include <M5EPD.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include "AppConfig.h"

#include "storage/config/CompositeConfigProvider.h"
#include "storage/config/NvsConfigProvider.h"
#include "storage/snapshot/NullAssignmentSnapshotStore.h"

#include "setup/SetupMode.h"
#include "setup/SetupEntryReason.h"
#include "ui/DashboardRenderer.h"
#include "domain/AssignmentService.h"
#include "net/NetworkManager.h"
#include "storage/snapshot/IAssignmentSnapshotStore.h"

M5EPD_Canvas canvas(&M5.EPD);

void setup()
{
    M5.begin();
    Serial.begin(115200);

    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);
    canvas.createCanvas(540, 960);

    NullAssignmentSnapshotStore snapshotStore;

    // Decide: setup vs runtime
    NvsConfigProvider nvs;

#if APP_DEV_MODE
    // DEV: auto-seed if empty
    CompositeConfigProvider cfgProvider;
    cfgProvider.seedFromHardcodedIfEmpty();
    AppConfig appConfig;
    cfgProvider.load(appConfig);
#else
    // PROD: if no valid NVS config -> setup portal
    AppConfig appConfig;
    if (!nvs.hasConfig() || !nvs.load(appConfig))
    {
        SetupMode setupMode(canvas, nvs);
        setupMode.run(SetupEntryReason::NoConfig);
        return;
    }

#endif

    Serial.printf("WiFi SSID in use: %s\n", appConfig.wifiSsid.c_str());
    Serial.printf("Course count: %d\n", (int)appConfig.courses.size());
    Serial.printf("Timezone offset: %ld\n", appConfig.timezoneOffsetSec);

    NetworkManager net(appConfig);
    bool netOk = net.begin();

    std::vector<AssignmentItem> items;
    bool hasItems = false;

    if (netOk)
    {
        hasItems = fetchAssignmentItems(appConfig, items);
        drawDashboard(canvas, items, hasItems);
        canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
    }
    else
    {
        SetupMode setupMode(canvas, nvs);
        setupMode.run(SetupEntryReason::WifiFailed);
        return;
    }

    Serial.println("Display Updated. Sleeping");
    delay(1000);
    M5.shutdown(7200);
}

void loop()
{
}