# SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# ------------------------------------------------------------------
# test-bed-utest-loadandparsesequencefile.pro - COPYRIGHT (C) 2019 BMS CIRCUITS
# ------------------------------------------------------------------

!versionAtLeast(QT_VERSION, 5.12.0): error("Requires Qt 5.12 or newer")

QT += testlib sql
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

CONFIG *= c++17
CONFIG *= file_copies

TEMPLATE = app

SEQTESTPATH = $$absolute_path(.)
ROOT = $$absolute_path(../..)
TESTBEDLIB_ROOT = $$absolute_path($$ROOT/test-bed-lib)

TESTBANKPLUGINDIR = $$absolute_path(../plugins-test/banks)

COPIES += bankFile

bankFile.files = $$files($$TESTBANKPLUGINDIR/plugin-simple*.dll)
bankFile.path = $$absolute_path($$DESTDIR/banks)

include($$TESTBEDLIB_ROOT/test-bed-lib-includes.pri)

# Test
INCLUDEPATH *= $$SEQTESTPATH
HEADERS *=  $$SEQTESTPATH/tst_sequencefile.hpp
SOURCES *=  $$SEQTESTPATH/tst_sequencefile.cpp

RESOURCES *= $$SEQTESTPATH/testfiles.qrc

unix {
    target.path = /opt/testbed
    INSTALLS += target
}
