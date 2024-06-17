# SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++17, if not returns an error when compiling
c++17 | c++2* {
    # At least C++17 - OK
} else {
    error($${TARGET} requires at least c++17)
}

!contains(DEFINES, BANK_CORE_SHARED) : error($${TARGET} requires BANK_CORE_SHARED)
!contains(DEFINES, HMI_HELPER_LIB) : error($${TARGET} requires HMI_HELPER_LIB)
!contains(DEFINES, JSON_BMS_LIB) : error($${TARGET} requires JSON_BMS_LIB)
!contains(DEFINES, TEST_BED_CORE) : error($${TARGET} requires TEST_BED_CORE)
!contains(DEFINES, TESTBEDPLUGIN_LIBRARY) : error($${TARGET} requires TESTBEDPLUGIN_LIBRARY)
!contains(DEFINES, WAIT_BMS_LIB) : error($${TARGET} requires WAIT_BMS_LIB)

DEFINES *= DETECTION_HELPER_LIB

DETECTION_HELPER_LIB_ROOT = $$absolute_path(.)

INCLUDEPATH *= $$DETECTION_HELPER_LIB_ROOT/

# Sources

## Modules
HEADERS *= $$DETECTION_HELPER_LIB_ROOT/modules/mixinboarddetection.hpp
SOURCES *= $$DETECTION_HELPER_LIB_ROOT/modules/mixinboarddetection.cpp

## Models
HEADERS *= $$DETECTION_HELPER_LIB_ROOT/models/detectionshareddata.hpp
SOURCES *= $$DETECTION_HELPER_LIB_ROOT/models/detectionshareddata.cpp
