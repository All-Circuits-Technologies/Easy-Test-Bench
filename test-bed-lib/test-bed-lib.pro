# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#-------------------------------------------------
#
# Project created by QtCreator 2019-06-24T11:13:24
#
#-------------------------------------------------

!versionAtLeast(QT_VERSION, 5.15.0): error("Requires Qt 5.15 or newer")

VERSION = 1.1.3

QT       -= gui
QT       += sql

BASENAME = $${basename(_PRO_FILE_)}
FILENAME = $$section(BASENAME, '.', 0, 0)

TARGET = $$qtLibraryTarget($${FILENAME})
TARGET_EXT =.dll

TEMPLATE = lib

ROOT = $$absolute_path(..)
ACTLIBS = $$absolute_path($$ROOT/actlibs)

include($$ACTLIBS/import-build-params.pri)

DESTDIR = $$DESTDIR_LIBS

CONFIG *= c++17

TESTBEDLIB_ROOT = $$absolute_path(.)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Allow to have the logs context in release
DEFINES *= QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$TESTBEDLIB_ROOT/test-bed-lib-includes.pri)

unix {
    target.path = /opt/testbed
    INSTALLS += target
}

# Database resources
RESOURCES *= $$TESTBEDLIB_ROOT/sql.qrc
