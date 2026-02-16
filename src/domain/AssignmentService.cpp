#include "AssignmentService.h"

#include <HTTPClient.h>
#include <time.h>
bool fetchAssignmentItems(const AppConfig &cfg, std::vector<AssignmentItem> &out)
{
    out.clear();

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

                    // Instead of drawing, push to vector
                    AssignmentItem item;
                    item.dateYmd = localDate;
                    item.timeHm = localTime;
                    item.courseName = cfg.courses[i].name;
                    item.title = title;
                    out.push_back(item);

                    // Keep your debug log if you want (still no behavior change)
                    Serial.printf("[%s] %s %s - %s\n",
                                  localDate.c_str(),
                                  localTime.c_str(),
                                  cfg.courses[i].name.c_str(),
                                  title.c_str());

                }

                searchPos += 12;
            }
        }
        http.end();
    }
    return !out.empty();
}