#pragma once
#include <vector>
#include "AppConfig.h"
#include "AssignmentItem.h"

bool fetchAssignmentItems(const AppConfig& cfg, std::vector<AssignmentItem>& out);