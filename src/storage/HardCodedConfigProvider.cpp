#include "HardCodedConfigProvider.h"
#include "secerts.h"

// Add your specific course URLs hardcoded for now
static Course myCourses[] = {
    {"Software Architecture", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=756156&token=ahng2k589kh86pee36c86"},
    {"Parallel Programming", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=760099&token=ahng2k589kh86pee36c86"},
    {"Lean Thinking", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=752846&token=ahng2k589kh86pee36c86"},
    {"Software Security", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=761367&token=ahng2k589kh86pee36c86"}
};

bool HardCodedConfigProvider::load(AppConfig &out)
{
    out.wifiSsid = WIFI_SSID;
    out.wifiPass = WIFI_PASS;
    out.timezoneOffsetSec = -7 * 3600;
    out.courses = myCourses;
    out.courseCount = (int)(sizeof(myCourses) / sizeof(myCourses[0]));
    return true;
}