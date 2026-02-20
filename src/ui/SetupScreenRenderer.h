#pragma once
#include <M5EPD.h>
#include <WiFi.h>
#include "setup/SetupEntryReason.h"

void drawSetupScreen(M5EPD_Canvas& canvas, const String& apSsid, const IPAddress& apIp,
                     SetupEntryReason setupEntryReason);