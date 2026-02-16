#pragma once
#include <vector>
#include "domain/Course.h"

struct AppConfig
{
    const char* wifiSsid;
    const char* wifiPass;

    long timezoneOffsetSec;

    Course* courses;
    int courseCount;
};
