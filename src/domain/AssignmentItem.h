#pragma once
#include <Arduino.h>

struct AssignmentItem
{
    String dateYmd;  // YYYYMMDD
    String timeHm;   // HH:MM
    String courseName;
    String title;
    uint32_t sortKey;  // YYYYMMDDHHMM as a number
};