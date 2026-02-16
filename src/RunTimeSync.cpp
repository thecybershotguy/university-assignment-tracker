#include "RunTimeSync.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

static void waitForValidTime(){
    struct tm now_tm;
        do {
            getLocalTime(&now_tm);
            delay(500);
        } while (now_tm.tm_year < 120);
}


void runRuntimeSync(const AppConfig &cfg, M5EPD_Canvas &canvas)
{
    WiFi.begin(cfg.wifiSsid, cfg.wifiPass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");

    configTime(cfg.timezoneOffsetSec, 0, "pool.ntp.org");
    Serial.print("Syncing NTP");
    waitForValidTime();
    Serial.println("\nTime Synced!");

    // 3) Calculate Yesterday/Today/Tomorrow (paste your current logic)
    struct tm now_tm;
    getLocalTime(&now_tm);
    time_t now_raw = mktime(&now_tm);

     char buf[12];
    strftime(buf, sizeof(buf), "%Y%m%d", &now_tm);
    String today = String(buf);

    time_t yesterday_raw = now_raw - 86400;
    struct tm* tm_yesterday = localtime(&yesterday_raw);
    strftime(buf, sizeof(buf), "%Y%m%d", tm_yesterday);
    String yesterday = String(buf);

    time_t tomorrow_raw = now_raw + 86400;
    struct tm* tm_tomorrow = localtime(&tomorrow_raw);
    strftime(buf, sizeof(buf), "%Y%m%d", tm_tomorrow);
    String tomorrow = String(buf);

    Serial.printf("System Dates: Yest=%s, Today=%s, Tom=%s\n",
                  yesterday.c_str(), today.c_str(), tomorrow.c_str());

    // 4) Battery header (paste your current logic)
    uint32_t vol = M5.getBatteryVoltage();
    int bat_pct = (vol - 3300) * 100 / (4350 - 3300);
    if (bat_pct > 100) bat_pct = 100;

    canvas.setTextSize(2);
    canvas.drawString("Bat: " + String(bat_pct) + "%", 430, 10);

    // 5) Fetch + parse + draw (move your for-loop here)
    int currentY = 45;
    String lastHeader = "";

    for (int i = 0; i < cfg.courseCount; i++) {
        HTTPClient http;
        http.begin(cfg.courses[i].url);

        if (http.GET() == HTTP_CODE_OK) {
            String payload = http.getString();
            int searchPos = 0;

            while ((searchPos = payload.indexOf("BEGIN:VEVENT", searchPos)) != -1) {
                int sumIdx = payload.indexOf("SUMMARY:", searchPos);
                int dateIdx = payload.indexOf("DTSTART", searchPos);

                if (sumIdx != -1 && dateIdx != -1) {
                    String title = payload.substring(sumIdx + 8, payload.indexOf("\r", sumIdx));

                    int timeSepIdx = payload.indexOf(":", dateIdx);

                    int yr   = payload.substring(timeSepIdx + 1,  timeSepIdx + 5).toInt();
                    int mo   = payload.substring(timeSepIdx + 5,  timeSepIdx + 7).toInt();
                    int dy   = payload.substring(timeSepIdx + 7,  timeSepIdx + 9).toInt();
                    int hrUTC= payload.substring(timeSepIdx + 10, timeSepIdx + 12).toInt();
                    int mn   = payload.substring(timeSepIdx + 12, timeSepIdx + 14).toInt();

                    struct tm deadline_tm = {0};
                    deadline_tm.tm_year = yr - 1900;
                    deadline_tm.tm_mon  = mo - 1;
                    deadline_tm.tm_mday = dy;
                    deadline_tm.tm_hour = hrUTC;
                    deadline_tm.tm_min  = mn;

                    // Keep same behavior, but use cfg offset instead of hardcoding 7h
                    time_t deadline_raw = mktime(&deadline_tm) + cfg.timezoneOffsetSec;
                    struct tm* local_deadline = localtime(&deadline_raw);

                    char dBuf[10], tBuf[10];
                    strftime(dBuf, sizeof(dBuf), "%Y%m%d", local_deadline);
                    strftime(tBuf, sizeof(tBuf), "%H:%M", local_deadline);

                    String localDate = String(dBuf);
                    String localTime = String(tBuf);

                    if (!localDate.startsWith("2026") || title.indexOf("Lecture") != -1) {
                        searchPos += 12;
                        continue;
                    }

                    if (localDate != lastHeader) {
                        String label;
                        if (localDate == yesterday) label = "YESTERDAY";
                        else if (localDate == today) label = "TODAY";
                        else if (localDate == tomorrow) label = "TOMORROW";
                        else label = localDate.substring(4,6) + "/" + localDate.substring(6,8);

                        currentY += 15;
                        canvas.setTextSize(4);
                        canvas.setTextColor(15);
                        canvas.drawString(label, 20, currentY);
                        currentY += 55;
                        canvas.drawFastHLine(20, currentY - 10, 500, 15);
                        lastHeader = localDate;
                    }

                    canvas.setTextSize(3);
                    if (localDate == today) {
                        canvas.fillRect(10, currentY, 520, 85, 15);
                        canvas.setTextColor(0);
                    } else {
                        canvas.setTextColor(15);
                    }

                    canvas.drawString(cfg.courses[i].name, 30, currentY + 10);
                    canvas.drawString(title.length() > 22 ? title.substring(0, 19) + "..." : title,
                                      30, currentY + 45);
                    canvas.drawString(localTime, 440, currentY + 10);

                    Serial.printf("[%s] %s %s - %s\n",
                                  localDate.c_str(), localTime.c_str(),
                                  cfg.courses[i].name.c_str(), title.c_str());

                    currentY += 95;
                }

                searchPos += 12;
                if (currentY > 900) break;
            }
        }

        http.end();
    }
    // 6) Push canvas (same as before)
    canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
}
