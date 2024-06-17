# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# ------------------------------------------------------------------
# test-bed-utest-variable-operations-plugin.pro - COPYRIGHT (C) 2019 BMS CIRCUITS
# ------------------------------------------------------------------

!versionAtLeast(QT_VERSION, 5.12.0): error("Requires Qt 5.12 or newer")

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

CONFIG *= c++17

TEMPLATE = app

ROOT            = $$absolute_path(../..)
TEST_ROOT       = $$absolute_path(.)
ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTUTILITIES = $$absolute_path($$ACTLIBS/qtutilities)

PLUGIN_ROOT     = $$absolute_path($$ROOT/plugins)
ABSTRACT_ELEMS_PATH = $$absolute_path($$ROOT/plugins/abstractpluginelements)

FILENAME = "variable-operations-plugin"
VERSION = "0.0.0"
DEFINES *= VERSION_STRING=\\\"$${VERSION}\\\"
DEFINES *= PLUGIN_NAME=\\\"$${FILENAME}\\\"

INCLUDEPATH *= $$ROOT
INCLUDEPATH *= $$ABSTRACT_ELEMS_PATH
HEADERS *= $$ABSTRACT_ELEMS_PATH/defaultbankglobalmanager.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/defaultbankglobalmanager.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/defaultbankinterface.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/defaultbankinterface.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/defaultsequenceinstancemodule.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/defaultsequenceinstancemodule.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/registering/registermoduleutility.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/registering/registermoduleutility.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/registering/registersequencemoduleutility.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/registering/registersequencemoduleutility.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/registering/registertopologymoduleutility.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/registering/registertopologymoduleutility.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/registering/registertopologymoduleutility.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/registering/registertopologymoduleutility.cpp

INCLUDEPATH *= $${PLUGIN_ROOT}
INCLUDEPATH *= $${PLUGIN_ROOT}/$${FILENAME}

# Operations
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/abaseoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/abaseoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/afromrefoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/afromrefoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/avalueassignmentoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/avalueassignmentoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/frommultirefnumberarithmeticoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/frommultirefnumberarithmeticoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/fromrefboolassignmentoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/fromrefboolassignmentoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/fromrefnumberarithmeticoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/fromrefnumberarithmeticoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/fromrefnumberassignmentoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/fromrefnumberassignmentoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/fromrefstringassignmentoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/fromrefstringassignmentoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/valueboolassignmentoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/valueboolassignmentoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/valuenumberassignmentoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/valuenumberassignmentoperator.cpp
HEADERS *= $${PLUGIN_ROOT}/$${FILENAME}/operations/valuestringassignmentoperator.hpp
SOURCES *= $${PLUGIN_ROOT}/$${FILENAME}/operations/valuestringassignmentoperator.cpp

INCLUDEPATH *= $$QTUTILITIES

include($$QTUTILITIES/definesutility/definesutility.pri)
include($$QTUTILITIES/jsonutility/jsonutility.pri)
include($$QTUTILITIES/collectionutility/collectionutility.pri)
include($$QTUTILITIES/byteutility/byteutility.pri)
include($$QTUTILITIES/numberutility/numberutility.pri)
include($$QTUTILITIES/waitutility/waitutility.pri)
include($$QTUTILITIES/threadutility/threadutility.pri)
include($$QTUTILITIES/fileutility/fileutility.pri)
include($$QTUTILITIES/logsutility/logsutility.pri)

include($$ROOT/testbedcore/testbedcore.pri)
include($$ROOT/bankcoreshared/bankcoreshared.pri)

INCLUDEPATH *= $${TEST_ROOT}
HEADERS *=  tst_variable_operations_plugin.hpp
HEADERS *=  fakecommoninterface.hpp
SOURCES *=  tst_variable_operations_plugin.cpp
SOURCES *=  fakecommoninterface.cpp

unix {
    target.path = /opt/testbed
    INSTALLS += target
}


