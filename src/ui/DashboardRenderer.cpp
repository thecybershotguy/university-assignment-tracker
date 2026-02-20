#include "DashboardRenderer.h"
#include <time.h>

static String ymdFromTm(const struct tm& t)
{
    char buf[12];
    strftime(buf, sizeof(buf), "%Y%m%d", &t);
    return String(buf);
}

void drawDashboard(M5EPD_Canvas& canvas, const std::vector<AssignmentItem>& items, bool hasItems)
{
    // ----------------------------
    // 1) Compute Today/Yesterday/Tomorrow labels
    // ----------------------------

    // get current local time from the ESP32 time subsystem
    struct tm now_tm;
    getLocalTime(&now_tm);

    // convert it to seconds since epoch (used for +/- 1 day math)
    time_t now_raw = mktime(&now_tm);

    // today in YYYYMMDD
    String today = ymdFromTm(now_tm);

    // yesterday: subtract 86400 seconds (24 hours)
    time_t yesterday_raw = now_raw - 86400;
    struct tm* tm_yesterday = localtime(&yesterday_raw);
    String yesterday = ymdFromTm(*tm_yesterday);

    // tomorrow: add 86400 seconds
    time_t tomorrow_raw = now_raw + 86400;
    struct tm* tm_tomorrow = localtime(&tomorrow_raw);
    String tomorrow = ymdFromTm(*tm_tomorrow);

    Serial.printf("System Dates: Yest=%s, Today=%s, Tom=%s\n", yesterday.c_str(), today.c_str(),
                  tomorrow.c_str());

    // ----------------------------
    // 2) Draw battery header (same logic you had)
    // ----------------------------

    // read battery voltage in millivolts
    uint32_t vol = M5.getBatteryVoltage();

    // map voltage range roughly to 0-100%
    int bat_pct = (vol - 3300) * 100 / (4350 - 3300);

    // clamp max to 100
    if (bat_pct > 100)
        bat_pct = 100;
    // clamp min to 0
    if (bat_pct < 0)
        bat_pct = 0;

    // text size for header
    canvas.setTextSize(2);

    canvas.setTextColor(15);
    canvas.drawString("ONLINE", 20, 10);

    if (!hasItems)
    {
        canvas.setTextSize(3);
        canvas.setTextColor(15);
        canvas.drawString("No assignments found", 60, 200);
        return;  // stop drawing further
    }

    // draw the battery percent at top right
    canvas.setTextColor(15);  // black
    canvas.drawString("Bat: " + String(bat_pct) + "%", 430, 10);

    // ----------------------------
    // 3) Draw assignments grouped by date
    // ----------------------------

    // where we start drawing assignment content
    int currentY = 45;

    // track the last date header we drew, to avoid repeating headers
    String lastHeader = "";

    // iterate through each assignment item (in whatever order it arrives)
    for (size_t k = 0; k < items.size(); k++)
    {
        const AssignmentItem& it = items[k];

        // if date changed, draw a new section header
        if (it.dateYmd != lastHeader)
        {
            String label;

            // choose special labels for yesterday/today/tomorrow
            if (it.dateYmd == yesterday)
                label = "YESTERDAY";
            else if (it.dateYmd == today)
                label = "TODAY";
            else if (it.dateYmd == tomorrow)
                label = "TOMORROW";
            else
            {
                // otherwise display MM/DD
                label = it.dateYmd.substring(4, 6) + "/" + it.dateYmd.substring(6, 8);
            }

            // spacing before header
            currentY += 15;

            // draw header text
            canvas.setTextSize(4);
            canvas.setTextColor(15);  // black
            canvas.drawString(label, 20, currentY);

            // draw line under header
            currentY += 55;
            canvas.drawFastHLine(20, currentY - 10, 500, 15);

            // remember this date
            lastHeader = it.dateYmd;
        }

        // ----------------------------
        // 4) Draw one assignment row/card (same look you had)
        // ----------------------------

        canvas.setTextSize(3);

        // highlight TODAY with a filled rectangle
        if (it.dateYmd == today)
        {
            // draw white filled box
            canvas.fillRect(10, currentY, 520, 85, 15);

            // set text to black on white
            canvas.setTextColor(0);  // white? (depends on your palette)
            // NOTE: In your original code you used 0 for "white text".
            // If the colors look inverted, swap 0 and 15 in this renderer.
        }
        else
        {
            // normal black text
            canvas.setTextColor(15);
        }

        // course name line
        canvas.drawString(it.courseName, 30, currentY + 10);

        // truncate title if long (same rule you used)
        String titleToDraw = it.title.length() > 22 ? it.title.substring(0, 19) + "..." : it.title;

        // title line
        canvas.drawString(titleToDraw, 30, currentY + 45);

        // time at right
        canvas.drawString(it.timeHm, 440, currentY + 10);

        // move down for next item
        currentY += 95;

        // stop drawing when screen is full (same as before)
        if (currentY > 900)
            break;
    }
}