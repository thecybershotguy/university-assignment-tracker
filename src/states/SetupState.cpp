#include "states/SetupState.h"

SetupState::SetupState(M5EPD_Canvas& canvas, NvsConfigProvider& nvs) : setupMode_(canvas, nvs)
{
}

AppState SetupState::tick(AppContext& ctx)
{
    // TODO: Replace restart-driven setup completion with an explicit state transition.
    setupMode_.begin(ctx.setupEntryReason);
    setupMode_.tick();
    return AppState::Setup;
}
