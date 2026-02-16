#include <M5EPD.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include "secerts.h"
#include "AppConfig.h"

// Add your specific course URLs here
Course myCourses[] = {
    {"Software Architecture", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=756156&token=ahng2k589kh86pee36c86"},
    {"Parallel Programming", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=760099&token=ahng2k589kh86pee36c86"},
    {"Lean Thinking", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=752846&token=ahng2k589kh86pee36c86"},
    {"Software Security", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=761367&token=ahng2k589kh86pee36c86"}
};

AppConfig appConfig = {
    WIFI_SSID, 
    WIFI_PASS, 
    -7 *3600,
    myCourses, 
    sizeof(myCourses) / sizeof(myCourses[0])
};

M5EPD_Canvas canvas(&M5.EPD);

void setup() {
    M5.begin();
    Serial.begin(115200);
    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);
    canvas.createCanvas(540, 960);
    
    // 1. Connect WiFi
    WiFi.begin(appConfig.wifiSsid, appConfig.wifiPass);
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.println("\nWiFi Connected!");

    // 2. Sync Time (Calgary MST is UTC-7)
    configTime(appConfig.timezoneOffsetSec, 0, "pool.ntp.org");

    // CRITICAL FIX: Wait until year is valid (not 1970)
    struct tm now_tm;
    Serial.print("Syncing NTP");
    do {
        getLocalTime(&now_tm);
        delay(500);
        Serial.print(".");
    } while (now_tm.tm_year < 120); // Year since 1900, so 120 = 2020
    Serial.println("\nTime Synced!");

    // 3. Calculate Yesterday/Today/Tomorrow
    time_t now_raw = mktime(&now_tm);
    char buf[12];
    
    strftime(buf, sizeof(buf), "%Y%m%d", &now_tm);
    String today = String(buf);

    time_t yesterday_raw = now_raw - 86400; // Subtract 24 hours
    struct tm * tm_yesterday = localtime(&yesterday_raw);
    strftime(buf, sizeof(buf), "%Y%m%d", tm_yesterday);
    String yesterday = String(buf);
    
    time_t tomorrow_raw = now_raw + 86400; // Add 24 hours
    struct tm * tm_tomorrow = localtime(&tomorrow_raw);
    strftime(buf, sizeof(buf), "%Y%m%d", tm_tomorrow);
    String tomorrow = String(buf);

    // Debug Log (Clean dates only)
    Serial.printf("System Dates: Yest=%s, Today=%s, Tom=%s\n", yesterday.c_str(), today.c_str(), tomorrow.c_str());

    // 4. Draw Battery Header
    uint32_t vol = M5.getBatteryVoltage();
    int bat_pct = (vol - 3300) * 100 / (4350 - 3300);
    if(bat_pct > 100) bat_pct = 100;
    canvas.setTextSize(2);
    canvas.drawString("Bat: " + String(bat_pct) + "%", 430, 10);

    int currentY = 45;
    String lastHeader = "";

    // 5. Fetch & Parse
    for (int i = 0; i < appConfig.courseCount; i++) {
        HTTPClient http;
        http.begin(appConfig.courses[i].url);
        if (http.GET() == HTTP_CODE_OK) {
            String payload = http.getString();
            int searchPos = 0;

            // Find every VEVENT
            while ((searchPos = payload.indexOf("BEGIN:VEVENT", searchPos)) != -1) {
                // STRICTLY use DTSTART (Due Date), ignore DTSTAMP (Creation Date)
                int sumIdx = payload.indexOf("SUMMARY:", searchPos);
                int dateIdx = payload.indexOf("DTSTART", searchPos);

                if (sumIdx != -1 && dateIdx != -1) {
                    // Extract strings
                    String title = payload.substring(sumIdx + 8, payload.indexOf("\r", sumIdx));
                    
                    // Robust Parsing: Find the colon ':' after DTSTART to handle TZID prefixes
                    int timeSepIdx = payload.indexOf(":", dateIdx);
                    
                    // Extract Raw UTC Components
                    int yr = payload.substring(timeSepIdx + 1, timeSepIdx + 5).toInt();
                    int mo = payload.substring(timeSepIdx + 5, timeSepIdx + 7).toInt();
                    int dy = payload.substring(timeSepIdx + 7, timeSepIdx + 9).toInt();
                    int hrUTC = payload.substring(timeSepIdx + 10, timeSepIdx + 12).toInt();
                    int mn = payload.substring(timeSepIdx + 12, timeSepIdx + 14).toInt();

                    // Convert to Calgary Time
                    struct tm deadline_tm = {0};
                    deadline_tm.tm_year = yr - 1900;
                    deadline_tm.tm_mon = mo - 1;
                    deadline_tm.tm_mday = dy;
                    deadline_tm.tm_hour = hrUTC;
                    deadline_tm.tm_min = mn;
                    
                    // Logic: Input is UTC. We treat it as local, then subtract 7 hours.
                    time_t deadline_raw = mktime(&deadline_tm) + appConfig.timezoneOffsetSec;
                    struct tm * local_deadline = localtime(&deadline_raw);
                    
                    char dBuf[10], tBuf[10];
                    strftime(dBuf, sizeof(dBuf), "%Y%m%d", local_deadline);
                    strftime(tBuf, sizeof(tBuf), "%H:%M", local_deadline);
                    String localDate = String(dBuf);
                    String localTime = String(tBuf);

                    // Filter: Only show 2026+ and ignore Lectures
                    if (!localDate.startsWith("2026") || title.indexOf("Lecture") != -1) { 
                        searchPos += 12; continue; 
                    }

                    // Dynamic Headers (Yesterday / Today / Tomorrow / Date)
                    if (localDate != lastHeader) {
                        String label = "";
                        if (localDate == yesterday) label = "YESTERDAY";
                        else if (localDate == today) label = "TODAY";
                        else if (localDate == tomorrow) label = "TOMORROW";
                        else label = localDate.substring(4,6) + "/" + localDate.substring(6,8); // MM/DD

                        currentY += 15;
                        canvas.setTextSize(4); canvas.setTextColor(15);
                        canvas.drawString(label, 20, currentY);
                        currentY += 55; canvas.drawFastHLine(20, currentY - 10, 500, 15);
                        lastHeader = localDate;
                    }

                    // Draw Assignment
                    canvas.setTextSize(3);
                    if (localDate == today) {
                        canvas.fillRect(10, currentY, 520, 85, 15);
                        canvas.setTextColor(0); // White text
                    } else {
                        canvas.setTextColor(15); // Black text
                    }

                    canvas.drawString(appConfig.courses[i].name, 30, currentY + 10);
                    // Truncate long titles
                    canvas.drawString(title.length() > 22 ? title.substring(0, 19) + "..." : title, 30, currentY + 45);
                    canvas.drawString(localTime, 440, currentY + 10);
                    
                    // Clean Debug Log
                    Serial.printf("[%s] %s %s - %s\n", localDate.c_str(), localTime.c_str(), appConfig.courses[i].name.c_str(), title.c_str());
                    
                    currentY += 95;
                }
                searchPos += 12;
                if (currentY > 900) break; // Stop if screen full
            }
        }
        http.end();
    }
    
    canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
    Serial.println("Updated. Sleeping...");
    delay(1000);
    M5.shutdown(7200); 
}

void loop() {}