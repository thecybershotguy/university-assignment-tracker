#pragma once
#include <M5EPD.h>
#include <WiFi.h>

void drawSetupScreen(M5EPD_Canvas& canvas, const String& apSsid, const IPAddress& apIp);