# SPDX-FileCopyrightText: 2021 - 2022 Damien Manceau <damien.manceau@allcircuits.com>
# SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

DEFINES *= EMTESTBENCH_BMS_LIB

EMTESTBENCH_LIB_ROOT = $$absolute_path(.)

INCLUDEPATH *= $$EMTESTBENCH_LIB_ROOT

# API
HEADERS *= $$EMTESTBENCH_LIB_ROOT/command/acmd.hpp
HEADERS *= $$EMTESTBENCH_LIB_ROOT/command/startcmd.hpp
HEADERS *= $$EMTESTBENCH_LIB_ROOT/command/terminatecmd.hpp

HEADERS *= $$EMTESTBENCH_LIB_ROOT/response/aresponse.hpp
HEADERS *= $$EMTESTBENCH_LIB_ROOT/response/backresponse.hpp
HEADERS *= $$EMTESTBENCH_LIB_ROOT/response/backresponsecategory.hpp
HEADERS *= $$EMTESTBENCH_LIB_ROOT/response/backresponsetype.hpp
HEADERS *= $$EMTESTBENCH_LIB_ROOT/response/idresponse.hpp
HEADERS *= $$EMTESTBENCH_LIB_ROOT/response/instrumentid.h

HEADERS *= $$EMTESTBENCH_LIB_ROOT/writeprocess.hpp

#SRC
SOURCES *= $$EMTESTBENCH_LIB_ROOT/command/acmd.cpp
SOURCES *= $$EMTESTBENCH_LIB_ROOT/command/startcmd.cpp
SOURCES *= $$EMTESTBENCH_LIB_ROOT/command/terminatecmd.cpp

SOURCES *= $$EMTESTBENCH_LIB_ROOT/response/aresponse.cpp
SOURCES *= $$EMTESTBENCH_LIB_ROOT/response/backresponse.cpp
SOURCES *= $$EMTESTBENCH_LIB_ROOT/response/backresponsecategory.cpp
SOURCES *= $$EMTESTBENCH_LIB_ROOT/response/backresponsetype.cpp
SOURCES *= $$EMTESTBENCH_LIB_ROOT/response/idresponse.cpp
SOURCES *= $$EMTESTBENCH_LIB_ROOT/response/instrumentid.cpp

SOURCES *= $$EMTESTBENCH_LIB_ROOT/writeprocess.cpp
