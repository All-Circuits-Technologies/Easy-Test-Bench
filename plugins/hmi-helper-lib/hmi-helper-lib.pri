# SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, BANK_CORE_SHARED) : error($${TARGET} requires BANK_CORE_SHARED)

DEFINES *= HMI_HELPER_LIB

HMI_HELPER_LIB_ROOT = $$absolute_path(.)

INCLUDEPATH *= $$HMI_HELPER_LIB_ROOT/

HEADERS *= $$HMI_HELPER_LIB_ROOT/hmidisplayfilebrowserview.hpp
SOURCES *= $$HMI_HELPER_LIB_ROOT/hmidisplayfilebrowserview.cpp
HEADERS *= $$HMI_HELPER_LIB_ROOT/hmidisplayhelper.hpp
SOURCES *= $$HMI_HELPER_LIB_ROOT/hmidisplayhelper.cpp
HEADERS *= $$HMI_HELPER_LIB_ROOT/hmidisplayinfinitewaitview.hpp
SOURCES *= $$HMI_HELPER_LIB_ROOT/hmidisplayinfinitewaitview.cpp
HEADERS *= $$HMI_HELPER_LIB_ROOT/hmidisplaysimpletestview.hpp
SOURCES *= $$HMI_HELPER_LIB_ROOT/hmidisplaysimpletestview.cpp
HEADERS *= $$HMI_HELPER_LIB_ROOT/hmidisplaystringinputview.hpp
SOURCES *= $$HMI_HELPER_LIB_ROOT/hmidisplaystringinputview.cpp
HEADERS *= $$HMI_HELPER_LIB_ROOT/hmidisplaystatus.hpp
SOURCES *= $$HMI_HELPER_LIB_ROOT/hmidisplaystatus.cpp

RESOURCES *= $$HMI_HELPER_LIB_ROOT/defaultimages.qrc
