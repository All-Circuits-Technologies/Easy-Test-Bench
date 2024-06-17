# SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, IMPORT_QTPEAKCANLIB) : error($${TARGET} requires IMPORT_QTPEAKCANLIB)
!contains(DEFINES, TEST_BED_CORE) : error($${TARGET} requires TEST_BED_CORE)

PLUGIN_PEAK_CAN_LIB_ROOT = $$absolute_path(.)

DEFINES *= PLUGIN_PEAK_CAN_LIB

INCLUDEPATH *= $$PLUGIN_PEAK_CAN_LIB_ROOT/

HEADERS *= $$PLUGIN_PEAK_CAN_LIB_ROOT/canmodulesutility.hpp
SOURCES *= $$PLUGIN_PEAK_CAN_LIB_ROOT/canmodulesutility.cpp
HEADERS *= $$PLUGIN_PEAK_CAN_LIB_ROOT/peakcanlibconstants.hpp
