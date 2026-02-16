#pragma once
#include "IAssignmentSnapshotStore.h"

class FsAssignmentSnapshotStore : public IAssignmentSnapshotStore
{
   public:
    explicit FsAssignmentSnapshotStore(const char* path = "/assignments_snapshot.txt") : path_(path)
    {
    }

    bool saveSnapshot(const String& payload, int64_t savedAtEpoch) override;
    bool loadSnapshot(String& outPayload, int64_t& outSavedAtEpoch) override;
    bool clearSnapshot() override;

   private:
    const char* path_;
};