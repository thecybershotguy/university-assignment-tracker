#pragma once
#include <vector>
#include <M5EPD.h>
#include "domain/AssignmentItem.h"

void drawDashboard(M5EPD_Canvas& canvas, const std::vector<AssignmentItem>& items, bool networkOk,
                   bool hasItems);