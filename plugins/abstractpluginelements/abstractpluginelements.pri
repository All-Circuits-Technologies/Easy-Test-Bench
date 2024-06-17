# SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

TEMPLATE = lib
CONFIG  *= plugin

QT -= gui
QT *= core

CONFIG *= c++17

# Allow to have the logs context in release
DEFINES *= QT_MESSAGELOGCONTEXT

DEFINES *= TESTBEDPLUGIN_LIBRARY

BASENAME = $${basename(_PRO_FILE_)}
FILENAME = $$section(BASENAME, '.', 0, 0)

TARGET = $$qtLibraryTarget($${FILENAME})
TARGET_EXT =.dll

ABSTRACT_ELEMS_PATH = $$absolute_path(.)
PLUGINS_FOLDER = $$absolute_path(..)
PLUGIN_PATH = $$absolute_path($$_PRO_FILE_PWD_)
ROOT = $$absolute_path(../..)
ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTUTILITIES = $$absolute_path($$ACTLIBS/qtutilities)

include($$ACTLIBS/import-build-params.pri)

!isEmpty(DESTDIR_PLUGINS) {
    DESTDIR = $$DESTDIR_PLUGINS
}

isEmpty( DESTDIR_PLUGINS ) {
    message("To help the building of banks, add an env variable in your Qt creator: \
             DESTDIR_PLUGINS, with the path of the directory to target.")
}

INSTALLS *= TARGET

isEmpty( VERSION ) {
    message("In order to have a correct version number returned by the plugin, it's necessary to\
             set, before the including of this .pri file, a value to the VERSION variable")
}

DEFINES *= VERSION_STRING=\\\"$${VERSION}\\\"
DEFINES *= PLUGIN_NAME=\\\"$${FILENAME}\\\"

INCLUDEPATH *= $$ABSTRACT_ELEMS_PATH
INCLUDEPATH *= $$PLUGINS_FOLDER
INCLUDEPATH *= $$PLUGIN_PATH
INCLUDEPATH *= $$QTUTILITIES
INCLUDEPATH *= $$ROOT

# Global class
HEADERS *= $$ABSTRACT_ELEMS_PATH/defaultbankglobalmanager.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/defaultbankglobalmanager.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/defaultbankinterface.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/defaultbankinterface.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/defaultsequenceinstancemodule.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/defaultsequenceinstancemodule.cpp

# Registering classes
HEADERS *= $$ABSTRACT_ELEMS_PATH/registering/registermoduleutility.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/registering/registermoduleutility.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/registering/registersequencemoduleutility.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/registering/registersequencemoduleutility.cpp
HEADERS *= $$ABSTRACT_ELEMS_PATH/registering/registertopologymoduleutility.hpp
SOURCES *= $$ABSTRACT_ELEMS_PATH/registering/registertopologymoduleutility.cpp

OTHER_FILES *= $$PLUGIN_PATH/$${FILENAME}.json

include($$QTUTILITIES/definesutility/definesutility.pri)
include($$QTUTILITIES/jsonutility/jsonutility.pri)
include($$QTUTILITIES/collectionutility/collectionutility.pri)
include($$QTUTILITIES/waitutility/waitutility.pri)
include($$QTUTILITIES/threadutility/threadutility.pri)
include($$QTUTILITIES/fileutility/fileutility.pri)
include($$QTUTILITIES/logsutility/logsutility.pri)
include($$QTUTILITIES/byteutility/byteutility.pri)
include($$QTUTILITIES/numberutility/numberutility.pri)
include($$QTUTILITIES/translationutility/translationutility.pri)

include($$ROOT/testbedcore/testbedcore.pri)
include($$ROOT/bankcoreshared/bankcoreshared.pri)

unix {
    target.path = /opt/testbed/banks
    INSTALLS += target
}
