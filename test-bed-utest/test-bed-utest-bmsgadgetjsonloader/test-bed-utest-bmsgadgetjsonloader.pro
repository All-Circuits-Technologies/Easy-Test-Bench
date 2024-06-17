# SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# ------------------------------------------------------------------
# test-bed-utest-bmsgadgetjsonloader.pro - COPYRIGHT (C) 2019 BMS CIRCUITS
# ------------------------------------------------------------------

!versionAtLeast(QT_VERSION, 5.12.0): error("Requires Qt 5.12 or newer")

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

CONFIG *= c++17

TEMPLATE = app

# Needed in order to directly includes the library classes in static
DEFINES += TESTBEDLIB_LIBRARY

ROOT = $$absolute_path(../..)
TESTBEDLIB_ROOT = $$absolute_path($$ROOT/test-bed-lib)
ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTUTILITIES = $$absolute_path($$ACTLIBS/qtutilities)

INCLUDEPATH *= $${TESTBEDLIB_ROOT}

HEADERS *= $${ROOT}/testbedcore/testbedglobal.hpp
HEADERS *= $${TESTBEDLIB_ROOT}/file/ajsonfile.hpp
SOURCES *= $${TESTBEDLIB_ROOT}/file/ajsonfile.cpp
HEADERS *= $${TESTBEDLIB_ROOT}/jsonparsers/ajsonparser.hpp
SOURCES *= $${TESTBEDLIB_ROOT}/jsonparsers/ajsonparser.cpp
HEADERS *= $${TESTBEDLIB_ROOT}/jsonparsers/bmsgadgetjsonloader.hpp
SOURCES *= $${TESTBEDLIB_ROOT}/jsonparsers/bmsgadgetjsonloader.cpp
SOURCES *= $${TESTBEDLIB_ROOT}/tools/qt/bmsgadget.cpp
HEADERS *= $${TESTBEDLIB_ROOT}/tools/qt/bmsgadget.hpp
SOURCES *= $${TESTBEDLIB_ROOT}/tools/qt/qobjecthelper.cpp
HEADERS *= $${TESTBEDLIB_ROOT}/tools/qt/qobjecthelper.hpp
HEADERS *= $$files($${TESTBEDLIB_ROOT}/topology/topologyfile*.hpp, false)
SOURCES *= $$files($${TESTBEDLIB_ROOT}/topology/topologyfile*.cpp, false)

INCLUDEPATH *= $${TESTBEDLIB_ROOT}/..
HEADERS *=  tst_bmsgadgetjsonloader.hpp
SOURCES *=  tst_bmsgadgetjsonloader.cpp

INCLUDEPATH *= $$QTUTILITIES

include($$QTUTILITIES/definesutility/definesutility.pri)
include($$QTUTILITIES/collectionutility/collectionutility.pri)
include($$QTUTILITIES/jsonutility/jsonutility.pri)

unix {
    target.path = /opt/testbed
    INSTALLS += target
}
