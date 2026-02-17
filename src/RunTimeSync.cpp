#include "RunTimeSync.h"
#include <HTTPClient.h>
#include "ui/DashboardRenderer.h"
#include "domain/AssignmentService.h"
#include "net/NetworkManager.h"
#include "storage/snapshot/IAssignmentSnapshotStore.h"

static void waitForValidTime()
{
    struct tm now_tm;
    do
    {
        getLocalTime(&now_tm);
        delay(500);
    } while (now_tm.tm_year < 120);
}

void runRuntimeSync(const AppConfig& cfg, M5EPD_Canvas& canvas,
                    IAssignmentSnapshotStore& snapshotStore)
{
    Serial.printf("WiFi SSID in use: %s\n", cfg.wifiSsid.c_str());
    Serial.printf("Course count: %d\n", (int)cfg.courses.size());
    Serial.printf("Timezone offset: %ld\n", cfg.timezoneOffsetSec);

    NetworkManager net(cfg);
    bool netOk = net.begin();

    std::vector<AssignmentItem> items;
    bool hasItems = false;

    if (netOk)
    {
        hasItems = fetchAssignmentItems(cfg, items);
    };

    drawDashboard(canvas, items, netOk, hasItems);
    canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
}
