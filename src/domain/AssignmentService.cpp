#include "AssignmentService.h"
#include <algorithm>
#include <HTTPClient.h>
#include <time.h>

bool fetchAssignmentItems(const AppConfig& cfg, std::vector<AssignmentItem>& out)
{
    out.clear();

    for (int i = 0; i < cfg.courseCount; i++)
    {
        struct tm now_tm;
        getLocalTime(&now_tm);
        time_t now_raw = mktime(&now_tm);

        time_t yesterday_raw = now_raw - 86400;
        struct tm* tm_yesterday = localtime(&yesterday_raw);

        char yBuf[12];
        strftime(yBuf, sizeof(yBuf), "%Y%m%d", tm_yesterday);
        String yesterday = String(yBuf);

        HTTPClient http;
        http.begin(cfg.courses[i].url);

        if (http.GET() == HTTP_CODE_OK)
        {
            String payload = http.getString();
            int searchPos = 0;

            while ((searchPos = payload.indexOf("BEGIN:VEVENT", searchPos)) != -1)
            {
                int sumIdx = payload.indexOf("SUMMARY:", searchPos);
                int dateIdx = payload.indexOf("DTSTART", searchPos);

                if (sumIdx != -1 && dateIdx != -1)
                {
                    String title = payload.substring(sumIdx + 8, payload.indexOf("\r", sumIdx));

                    int timeSepIdx = payload.indexOf(":", dateIdx);

                    int yr = payload.substring(timeSepIdx + 1, timeSepIdx + 5).toInt();
                    int mo = payload.substring(timeSepIdx + 5, timeSepIdx + 7).toInt();
                    int dy = payload.substring(timeSepIdx + 7, timeSepIdx + 9).toInt();
                    int hrUTC = payload.substring(timeSepIdx + 10, timeSepIdx + 12).toInt();
                    int mn = payload.substring(timeSepIdx + 12, timeSepIdx + 14).toInt();

                    struct tm deadline_tm = {0};
                    deadline_tm.tm_year = yr - 1900;
                    deadline_tm.tm_mon = mo - 1;
                    deadline_tm.tm_mday = dy;
                    deadline_tm.tm_hour = hrUTC;
                    deadline_tm.tm_min = mn;

                    // Keep same behavior, but use cfg offset instead of hardcoding 7h
                    time_t deadline_raw = mktime(&deadline_tm) + cfg.timezoneOffsetSec;
                    struct tm* local_deadline = localtime(&deadline_raw);

                    char dBuf[10], tBuf[10];
                    strftime(dBuf, sizeof(dBuf), "%Y%m%d", local_deadline);
                    strftime(tBuf, sizeof(tBuf), "%H:%M", local_deadline);

                    String localDate = String(dBuf);
                    String localTime = String(tBuf);

                    if (localDate < yesterday)
                    {
                        searchPos += 12;
                        continue;
                    }

                    if (!localDate.startsWith("2026"))
                    {
                        searchPos += 12;
                        continue;
                    }

                    uint32_t y = localDate.substring(0, 4).toInt();
                    uint32_t m = localDate.substring(4, 6).toInt();
                    uint32_t d = localDate.substring(6, 8).toInt();
                    uint32_t hh = localTime.substring(0, 2).toInt();
                    uint32_t mm = localTime.substring(3, 5).toInt();

                    // Instead of drawing, push to vector
                    AssignmentItem item;
                    item.dateYmd = localDate;
                    item.timeHm = localTime;
                    item.courseName = cfg.courses[i].name;
                    item.title = title;
                    item.sortKey = (((y * 100 + m) * 100 + d) * 100 + hh) * 100 + mm;
                    out.push_back(item);

                    // Keep your debug log if you want (still no behavior change)
                    Serial.printf("[%s] %s %s - %s\n", localDate.c_str(), localTime.c_str(),
                                  cfg.courses[i].name.c_str(), title.c_str());
                }

                searchPos += 12;
            }
        }
        http.end();
    }
    std::sort(out.begin(), out.end(),
              [](const AssignmentItem& a, const AssignmentItem& b)
              {
                  if (a.sortKey != b.sortKey)
                      return a.sortKey < b.sortKey;
                  if (a.courseName != b.courseName)
                      return a.courseName < b.courseName;
                  return a.title < b.title;
              });
    return !out.empty();
}