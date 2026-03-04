#include <M5EPD.h>
#include "AppConfig.h"
#include "states/AppContext.h"
#include "states/AppState.h"
#include "states/BootState.h"
#include "states/RuntimeState.h"
#include "states/SetupState.h"
#include "states/SleepState.h"
#include "storage/config/NvsConfigProvider.h"

M5EPD_Canvas canvas(&M5.EPD);
NvsConfigProvider nvs;
AppConfig appConfig;
AppContext appContext{canvas, nvs, appConfig};
BootState bootState;
SetupState setupState(canvas, nvs);
RuntimeState runtimeState;
SleepState sleepState;
AppState appState = AppState::Boot;

void setup()
{
    M5.begin();
    Serial.begin(115200);

    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);
    canvas.createCanvas(540, 960);
    appState = AppState::Boot;
}

void loop()
{
    switch (appState)
    {
        case AppState::Boot:
            appState = bootState.tick(appContext);
            break;

        case AppState::Setup:
            appState = setupState.tick(appContext);
            break;

        case AppState::Runtime:
            appState = runtimeState.tick(appContext);
            break;

        case AppState::Sleep:
            appState = sleepState.tick(appContext);
            break;
    }
}
