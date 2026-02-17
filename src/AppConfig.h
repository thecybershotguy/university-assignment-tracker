#pragma once
#include <vector>
#include "domain/Course.h"

struct AppConfig
{
    String wifiSsid;
    String wifiPass;

    long timezoneOffsetSec = 0;

    std::vector<Course> courses;
};
