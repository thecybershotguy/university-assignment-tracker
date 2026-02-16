#include "RunTimeSync.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include "ui/DashboardRenderer.h"
#include "domain/AssignmentService.h"

static void waitForValidTime(){
    struct tm now_tm;
        do {
            getLocalTime(&now_tm);
            delay(500);
        } while (now_tm.tm_year < 120);
}


void runRuntimeSync(const AppConfig &cfg, M5EPD_Canvas &canvas)
{
    // 1) Connect WiFi
    WiFi.begin(cfg.wifiSsid, cfg.wifiPass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    // 2) Sync NTP
    configTime(cfg.timezoneOffsetSec, 0, "pool.ntp.org");
    Serial.print("Syncing NTP");
    waitForValidTime();
    Serial.println("\nTime Synced!");

    std::vector<AssignmentItem> items;
    fetchAssignmentItems(cfg, items);
    drawDashboard(canvas, items);
    canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
}
