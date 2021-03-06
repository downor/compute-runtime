/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once
#include "shared/test/unit_test/mocks/mock_compiler_interface.h"
#include "shared/test/unit_test/mocks/mock_device.h"
#include "shared/test/unit_test/mocks/mock_os_library.h"

#include "opencl/test/unit_test/mocks/mock_source_level_debugger.h"

#include "level_zero/core/source/cmdqueue/cmdqueue_hw.h"
#include "level_zero/core/source/fence/fence.h"
#include "level_zero/core/source/module/module.h"
#include "level_zero/core/test/unit_tests/mocks/mock_built_ins.h"
#include "level_zero/core/test/unit_tests/mocks/mock_device.h"
#include "level_zero/core/test/unit_tests/mocks/mock_driver.h"
#include "level_zero/core/test/unit_tests/mocks/mock_memory_manager.h"

namespace L0 {
namespace ult {

struct ActiveDebuggerFixture {
    void SetUp() { // NOLINT(readability-identifier-naming)
        auto executionEnvironment = new NEO::ExecutionEnvironment();
        auto mockBuiltIns = new MockBuiltins();
        executionEnvironment->prepareRootDeviceEnvironments(1);
        executionEnvironment->rootDeviceEnvironments[0]->builtins.reset(mockBuiltIns);
        executionEnvironment->rootDeviceEnvironments[0]->setHwInfo(defaultHwInfo.get());

        debugger = new MockActiveSourceLevelDebugger(new MockOsLibrary);
        executionEnvironment->rootDeviceEnvironments[0]->debugger.reset(debugger);
        executionEnvironment->initializeMemoryManager();

        device = NEO::MockDevice::create<NEO::MockDevice>(executionEnvironment, 0u);
        device->setDebuggerActive(true);

        std::vector<std::unique_ptr<NEO::Device>> devices;
        devices.push_back(std::unique_ptr<NEO::Device>(device));

        auto driverHandleUlt = whitebox_cast(DriverHandle::create(std::move(devices)));
        driverHandle.reset(driverHandleUlt);

        ASSERT_NE(nullptr, driverHandle);

        ze_device_handle_t hDevice;
        uint32_t count = 1;
        ze_result_t result = driverHandle->getDevice(&count, &hDevice);
        EXPECT_EQ(ZE_RESULT_SUCCESS, result);
        deviceL0 = L0::Device::fromHandle(hDevice);
        ASSERT_NE(nullptr, deviceL0);
    }
    void TearDown() { // NOLINT(readability-identifier-naming)
    }

    std::unique_ptr<L0::ult::WhiteBox<L0::DriverHandle>> driverHandle;
    NEO::MockDevice *device = nullptr;
    L0::Device *deviceL0;
    MockActiveSourceLevelDebugger *debugger = nullptr;
};
} // namespace ult
} // namespace L0
