# SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, JSON_BMS_LIB) : error($${TARGET} requires JSON_BMS_LIB)
!contains(DEFINES, TEST_BED_CORE) : error($${TARGET} requires TEST_BED_CORE)

DEFINES *= BANK_CORE_SHARED

BANK_CORE_ROOT = $$absolute_path(.)

INCLUDEPATH *= $$BANK_CORE_ROOT

# Shared elements
HEADERS *= $$BANK_CORE_ROOT/usebylib/ibankinterface.hpp
SOURCES *= $$BANK_CORE_ROOT/usebylib/ibankinterface.cpp
HEADERS *= $$BANK_CORE_ROOT/usebylib/iplugin.hpp
SOURCES *= $$BANK_CORE_ROOT/usebylib/iplugin.cpp
HEADERS *= $$BANK_CORE_ROOT/usebylib/isequenceinstancemodule.hpp
SOURCES *= $$BANK_CORE_ROOT/usebylib/isequenceinstancemodule.cpp
HEADERS *= $$BANK_CORE_ROOT/usebylib/itopologyinstancemodule.hpp
SOURCES *= $$BANK_CORE_ROOT/usebylib/itopologyinstancemodule.cpp
# Specific shared elements for intermediary elements like: matrix
HEADERS *= $$BANK_CORE_ROOT/usebylib/intermediary/iintertopologyinstancemodule.hpp
SOURCES *= $$BANK_CORE_ROOT/usebylib/intermediary/iintertopologyinstancemodule.cpp

HEADERS *= $$BANK_CORE_ROOT/usebyplugin/icommoninterface.hpp
SOURCES *= $$BANK_CORE_ROOT/usebyplugin/icommoninterface.cpp
HEADERS *= $$BANK_CORE_ROOT/usebyplugin/isequencelogmanager.hpp
SOURCES *= $$BANK_CORE_ROOT/usebyplugin/isequencelogmanager.cpp
