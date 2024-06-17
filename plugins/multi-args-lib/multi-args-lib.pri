# SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, JSON_BMS_LIB) : error("$${TARGET} (MULTI_ARGS_LIB) requires JSON_BMS_LIB")
!contains(DEFINES, TEST_BED_CORE) : error("$${TARGET} (MULTI_ARGS_LIB) requires TEST_BED_CORE")
!contains(DEFINES, TESTBEDPLUGIN_LIBRARY) {
    error("$${TARGET} (MULTI_ARGS_LIB) requires TESTBEDPLUGIN_LIBRARY")
}

DEFINES *= MULTI_ARGS_LIB

MULTI_ARGS_LIB_ROOT = $$absolute_path(.)

INCLUDEPATH *= $$MULTI_ARGS_LIB_ROOT/

HEADERS *= $$MULTI_ARGS_LIB_ROOT/mixinmultistringargsmodule.hpp
SOURCES *= $$MULTI_ARGS_LIB_ROOT/mixinmultistringargsmodule.cpp
