#include "states/RuntimeState.h"
#include "domain/AssignmentService.h"
#include "net/NetworkManager.h"
#include "ui/DashboardRenderer.h"

AppState RuntimeState::tick(AppContext& ctx)
{
    NetworkManager net(ctx.appConfig);
    if (!net.begin())
    {
        ctx.setupEntryReason = SetupEntryReason::WifiFailed;
        return AppState::Setup;
    }

    std::vector<AssignmentItem> items;
    bool hasItems = fetchAssignmentItems(ctx.appConfig, items);
    drawDashboard(ctx.canvas, items, hasItems);
    ctx.canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);

    Serial.println("Display Updated. Sleeping");
    return AppState::Sleep;
}
