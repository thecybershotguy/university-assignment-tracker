#pragma once
#include <M5EPD.h>
#include "AppConfig.h"
#include "storage/snapshot/IAssignmentSnapshotStore.h"

void runRuntimeSync(const AppConfig& cfg, M5EPD_Canvas& canvas,
                    IAssignmentSnapshotStore& snapshotStore);