#include "states/SleepState.h"

AppState SleepState::tick(AppContext&)
{
    delay(1000);
    M5.shutdown(7200);
    return AppState::Sleep;
}
