# SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# ------------------------------------------------------------------
# test-bed-utest-serial-link.pro - COPYRIGHT (C) 2019 BMS CIRCUITS
# ------------------------------------------------------------------

!versionAtLeast(QT_VERSION, 5.12.0): error("Requires Qt 5.12 or newer")

QT += testlib sql
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

CONFIG *= c++17

TEMPLATE = app

ROOT = $$absolute_path(../..)
ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTUTILITIES = $$absolute_path($$ACTLIBS/qtutilities)

unix {
    target.path = /opt/testbed
    INSTALLS += target
}

# Test files
HEADERS *= tst_seriallinkplugin.hpp
SOURCES *= tst_seriallinkplugin.cpp
RESOURCES *= utest-serial-link-sequences.qrc

# Required elements to properly load serial-link plugin
TESTBEDLIB_ROOT = $$absolute_path($${ROOT}/test-bed-lib)
INCLUDEPATH *= $${ROOT}
INCLUDEPATH *= $${TESTBEDLIB_ROOT}
INCLUDEPATH *= $$QTUTILITIES

DEFINES += TESTBEDLIB_LIBRARY

include($$TESTBEDLIB_ROOT/test-bed-lib-includes.pri)
