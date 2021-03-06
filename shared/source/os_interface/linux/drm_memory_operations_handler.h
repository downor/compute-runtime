/*
 * Copyright (C) 2019-2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once
#include "shared/source/memory_manager/memory_operations_handler.h"

#include <mutex>
#include <unordered_set>

namespace NEO {
class DrmMemoryOperationsHandler : public MemoryOperationsHandler {
  public:
    DrmMemoryOperationsHandler();
    ~DrmMemoryOperationsHandler() override;

    MemoryOperationsStatus makeResident(ArrayRef<GraphicsAllocation *> gfxAllocations) override;
    MemoryOperationsStatus evict(GraphicsAllocation &gfxAllocation) override;
    MemoryOperationsStatus isResident(GraphicsAllocation &gfxAllocation) override;

    std::unordered_set<GraphicsAllocation *> getResidencySet();

  protected:
    std::unordered_set<GraphicsAllocation *> residency;
    std::mutex mutex;
};
} // namespace NEO
