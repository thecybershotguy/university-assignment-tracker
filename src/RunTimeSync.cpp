#include "RunTimeSync.h"
#include <HTTPClient.h>
#include "ui/DashboardRenderer.h"
#include "domain/AssignmentService.h"
#include "net/NetworkManager.h"

static void waitForValidTime(){
    struct tm now_tm;
        do {
            getLocalTime(&now_tm);
            delay(500);
        } while (now_tm.tm_year < 120);
}


void runRuntimeSync(const AppConfig &cfg, M5EPD_Canvas &canvas)
{
    NetworkManager net(cfg);
    net.begin();
    std::vector<AssignmentItem> items;
    fetchAssignmentItems(cfg, items);
    drawDashboard(canvas, items);
    canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
}
