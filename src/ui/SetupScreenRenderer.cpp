#include "ui/SetupScreenRenderer.h"

static void safePushCanvas(M5EPD_Canvas& canvas)
{
    canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
}

void drawSetupScreen(M5EPD_Canvas& canvas, const String& apSsid, const IPAddress& apIp)
{
    const int W = 540;
    const int H = 960;
    const int M = 18;
    const int CONTENT_W = W - 2 * M;

    // M5EPD_Canvas grayscale: 0 = white, 15 = black (darkest)
    const uint16_t EINK_WHITE = 0;
    const uint16_t EINK_BLACK = 15;

    canvas.fillCanvas(EINK_WHITE);

    // ---------------- Header ----------------
    const int headerH = 95;
    canvas.fillRect(0, 0, W, headerH, EINK_BLACK);
    canvas.drawRect(0, 0, W, headerH, EINK_BLACK);

    canvas.setTextDatum(MC_DATUM);
    canvas.setTextColor(EINK_WHITE);
    canvas.setTextSize(5);
    canvas.drawString("SETUP MODE", W / 2, headerH / 2);
    canvas.setTextDatum(TL_DATUM);

    // ---------------- Step 1 ----------------
    int y = headerH + 18;

    canvas.setTextColor(EINK_BLACK);
    canvas.setTextSize(2);
    canvas.drawString("1) Connect to this WiFi network:", M, y);
    y += 34;

    // ---------------- SSID box ----------------
    const int boxH = 120;
    canvas.drawRect(M, y, CONTENT_W, boxH, EINK_BLACK);

    canvas.setTextDatum(MC_DATUM);
    canvas.setTextSize(4);  // SSID big
    canvas.drawString(apSsid, W / 2, y + boxH / 2);
    canvas.setTextDatum(TL_DATUM);

    y += boxH + 28;

    // ---------------- Step 2 ----------------
    canvas.setTextSize(2);
    canvas.drawString("2) Open this address in your browser:", M, y);
    y += 34;

    // ---------------- IP box ----------------
    canvas.drawRect(M, y, CONTENT_W, boxH, EINK_BLACK);

    canvas.setTextDatum(MC_DATUM);
    canvas.setTextSize(4);  // IP same size as SSID
    canvas.drawString(apIp.toString(), W / 2, y + boxH / 2);
    canvas.setTextDatum(TL_DATUM);

    y += boxH + 32;

    // ---------------- Step 3 + footer ----------------
    canvas.setTextSize(2);
    canvas.drawString("3) Fill WiFi + iCal URLs on the page.", M, y);
    y += 30;

    canvas.setTextSize(2);
    canvas.drawString("4) Press Save.", M, y);
    y += 30;

    canvas.setTextSize(2);
    canvas.drawString("5) Device will reboot automatically.", M, y);

    // Tip at bottom (3 clean short lines)
    canvas.setTextSize(2);
    canvas.drawString("Tip:", M, H - 90);
    canvas.drawString("If the page doesn't load,", M, H - 60);
    canvas.drawString("Check if you're connected to setup WiFi.", M, H - 30);

    safePushCanvas(canvas);
}