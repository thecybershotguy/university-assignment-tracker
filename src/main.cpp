#include <M5EPD.h>
#include <WiFi.h>
#include <time.h>
#include "AppConfig.h"
#include "RunTimeSync.h"

#include "storage/config/CompositeConfigProvider.h"
#include "storage/config/NvsConfigProvider.h"
#include "storage/snapshot/NullAssignmentSnapshotStore.h"

#include "setup/SetupPortal.h"

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
    runRuntimeSync(appConfig, canvas, snapshotStore);

#else
    // PROD: if no valid NVS config -> setup portal
    AppConfig appConfig;
    if (!nvs.hasConfig() || !nvs.load(appConfig))
    {
        SetupPortal portal(canvas, nvs);
        portal.run();  // blocks until save/clear triggers restart
        return;
    }

    runRuntimeSync(appConfig, canvas, snapshotStore);
#endif

    Serial.println("Display Updated. Sleeping");
    delay(1000);
    M5.shutdown(7200);
}

void loop()
{
}