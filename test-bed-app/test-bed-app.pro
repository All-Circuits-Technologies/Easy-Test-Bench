# SPDX-FileCopyrightText: 2021 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Raphaël Aurejac <raphael.aurejac@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

!versionAtLeast(QT_VERSION, 5.15.0): error("Requires Qt 5.15 or newer")

VERSION = 1.0.3
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QT += core qml quick

APP_PATH = $$absolute_path(.)
ROOT = $$absolute_path(..)
ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTUTILITIES = $$absolute_path($$ACTLIBS/qtutilities)

SUPPORTED_LAN_KEYS = en fr

include($$ACTLIBS/import-build-params.pri)

DESTDIR = $$DESTDIR_LIBS

CONFIG *= c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Allow to have the logs context in release
DEFINES *= QT_MESSAGELOGCONTEXT

DEPENDPATH *= $$APP_PATH
DEPENDPATH *= $$ROOT/test-bed-lib
INCLUDEPATH *= $$ROOT/test-bed-lib
DEPENDPATH *= $$ROOT
INCLUDEPATH *= $$ROOT
INCLUDEPATH *= $$QTUTILITIES

LIBS *= -L$$DESTDIR_LIBS -l$$qtLibraryTarget(test-bed-lib)

SOURCES *= $$APP_PATH/main.cpp

# blocs
HEADERS *= $$APP_PATH/blocs/abstractbloc.hpp
SOURCES *= $$APP_PATH/blocs/abstractbloc.cpp
HEADERS *= $$APP_PATH/blocs/blocsregistrator.hpp
SOURCES *= $$APP_PATH/blocs/blocsregistrator.cpp
HEADERS *= $$APP_PATH/blocs/loginbloc.hpp
SOURCES *= $$APP_PATH/blocs/loginbloc.cpp
HEADERS *= $$APP_PATH/blocs/creditspopupbloc.hpp
SOURCES *= $$APP_PATH/blocs/creditspopupbloc.cpp
HEADERS *= $$APP_PATH/blocs/topbarbloc.hpp
SOURCES *= $$APP_PATH/blocs/topbarbloc.cpp
HEADERS *= $$APP_PATH/blocs/abasepagebloc.hpp
SOURCES *= $$APP_PATH/blocs/abasepagebloc.cpp
HEADERS *= $$APP_PATH/blocs/afileselectionbloc.hpp
SOURCES *= $$APP_PATH/blocs/afileselectionbloc.cpp
HEADERS *= $$APP_PATH/blocs/projectselectionbloc.hpp
SOURCES *= $$APP_PATH/blocs/projectselectionbloc.cpp
HEADERS *= $$APP_PATH/blocs/recbuttonbloc.hpp
SOURCES *= $$APP_PATH/blocs/recbuttonbloc.cpp
HEADERS *= $$APP_PATH/blocs/sequenceselectionbloc.hpp
SOURCES *= $$APP_PATH/blocs/sequenceselectionbloc.cpp
HEADERS *= $$APP_PATH/blocs/sequencerunbloc.hpp
SOURCES *= $$APP_PATH/blocs/sequencerunbloc.cpp
#   # companions
HEADERS *= $$APP_PATH/blocs/companions/errorhandlercompanion.hpp
SOURCES *= $$APP_PATH/blocs/companions/errorhandlercompanion.cpp
HEADERS *= $$APP_PATH/blocs/companions/menucompanion.hpp
SOURCES *= $$APP_PATH/blocs/companions/menucompanion.cpp
HEADERS *= $$APP_PATH/blocs/companions/menuitemname.hpp
SOURCES *= $$APP_PATH/blocs/companions/menuitemname.cpp
#   # plugins
HEADERS *= $$APP_PATH/blocs/plugins/sequencepluginsbloc.hpp
SOURCES *= $$APP_PATH/blocs/plugins/sequencepluginsbloc.cpp
HEADERS *= $$APP_PATH/blocs/plugins/aplugininputbloc.hpp
SOURCES *= $$APP_PATH/blocs/plugins/aplugininputbloc.cpp
HEADERS *= $$APP_PATH/blocs/plugins/radiolistbuttonsinputbloc.hpp
SOURCES *= $$APP_PATH/blocs/plugins/radiolistbuttonsinputbloc.cpp
HEADERS *= $$APP_PATH/blocs/plugins/selectfileinputbloc.hpp
SOURCES *= $$APP_PATH/blocs/plugins/selectfileinputbloc.cpp
HEADERS *= $$APP_PATH/blocs/plugins/stringinputbloc.hpp
SOURCES *= $$APP_PATH/blocs/plugins/stringinputbloc.cpp

# core
HEADERS *= $$APP_PATH/core/hmimanager.hpp
SOURCES *= $$APP_PATH/core/hmimanager.cpp
HEADERS *= $$APP_PATH/core/hmiconstants.hpp
HEADERS *= $$APP_PATH/core/configurationmanager.hpp
SOURCES *= $$APP_PATH/core/configurationmanager.cpp
HEADERS *= $$APP_PATH/core/configurationkeys.hpp
SOURCES *= $$APP_PATH/core/configurationkeys.cpp

# interfaces
HEADERS *= $$APP_PATH/interfaces/appinterfacemanager.hpp
SOURCES *= $$APP_PATH/interfaces/appinterfacemanager.cpp

# models
HEADERS *= $$APP_PATH/models/filelistitemmodel.hpp
SOURCES *= $$APP_PATH/models/filelistitemmodel.cpp
HEADERS *= $$APP_PATH/models/formtextfieldmodel.hpp
SOURCES *= $$APP_PATH/models/formtextfieldmodel.cpp
HEADERS *= $$APP_PATH/models/genericlistmodel.hpp
HEADERS *= $$APP_PATH/models/genericlistmodeldata.hpp
SOURCES *= $$APP_PATH/models/genericlistmodeldata.cpp
HEADERS *= $$APP_PATH/models/logviewitemmodel.hpp
SOURCES *= $$APP_PATH/models/logviewitemmodel.cpp
HEADERS *= $$APP_PATH/models/menuitemmodel.hpp
SOURCES *= $$APP_PATH/models/menuitemmodel.cpp
HEADERS *= $$APP_PATH/models/modelsregistrator.hpp
SOURCES *= $$APP_PATH/models/modelsregistrator.cpp
HEADERS *= $$APP_PATH/models/scrollablepopupmodel.hpp
SOURCES *= $$APP_PATH/models/scrollablepopupmodel.cpp
#   # plugins
HEADERS *= $$APP_PATH/models/plugins/aplugininputmodel.hpp
SOURCES *= $$APP_PATH/models/plugins/aplugininputmodel.cpp
HEADERS *= $$APP_PATH/models/plugins/buttoninputmodel.hpp
SOURCES *= $$APP_PATH/models/plugins/buttoninputmodel.cpp
HEADERS *= $$APP_PATH/models/plugins/stringinputmodel.hpp
SOURCES *= $$APP_PATH/models/plugins/stringinputmodel.cpp
HEADERS *= $$APP_PATH/models/plugins/selectfileinputmodel.hpp
SOURCES *= $$APP_PATH/models/plugins/selectfileinputmodel.cpp
HEADERS *= $$APP_PATH/models/plugins/radiolistbuttonsinputmodel.hpp
SOURCES *= $$APP_PATH/models/plugins/radiolistbuttonsinputmodel.cpp

# services
HEADERS *= $$APP_PATH/services/servicesregistrator.hpp
SOURCES *= $$APP_PATH/services/servicesregistrator.cpp
HEADERS *= $$APP_PATH/services/sizemanager.hpp
SOURCES *= $$APP_PATH/services/sizemanager.cpp
#   # navigation
HEADERS *= $$APP_PATH/services/navigation/navigationbehavior.hpp
SOURCES *= $$APP_PATH/services/navigation/navigationbehavior.cpp
HEADERS *= $$APP_PATH/services/navigation/navigationpagetype.hpp
SOURCES *= $$APP_PATH/services/navigation/navigationpagetype.cpp
HEADERS *= $$APP_PATH/services/navigation/navigationmanager.hpp
SOURCES *= $$APP_PATH/services/navigation/navigationmanager.cpp

# types
HEADERS *= $$APP_PATH/types/imagetype.hpp
SOURCES *= $$APP_PATH/types/imagetype.cpp
HEADERS *= $$APP_PATH/types/pluginwidgettype.hpp
SOURCES *= $$APP_PATH/types/pluginwidgettype.cpp
HEADERS *= $$APP_PATH/types/popupbtntype.hpp
SOURCES *= $$APP_PATH/types/popupbtntype.cpp
HEADERS *= $$APP_PATH/types/typesregistrator.hpp
SOURCES *= $$APP_PATH/types/typesregistrator.cpp

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

include($$QTUTILITIES/definesutility/definesutility.pri)
include($$QTUTILITIES/colorutility/colorutility.pri)
include($$QTUTILITIES/jsonutility/jsonutility.pri)
include($$QTUTILITIES/collectionutility/collectionutility.pri)
include($$QTUTILITIES/waitutility/waitutility.pri)
include($$QTUTILITIES/threadutility/threadutility.pri)
include($$QTUTILITIES/fileutility/fileutility.pri)
include($$QTUTILITIES/processutility/processutility.pri)
include($$QTUTILITIES/logsutility/logsutility.pri)
include($$QTUTILITIES/managersutility/managersutility.pri)
include($$QTUTILITIES/yamlutility/yamlutility.pri)
include($$QTUTILITIES/byteutility/byteutility.pri)
include($$QTUTILITIES/numberutility/numberutility.pri)
include($$QTUTILITIES/ticutility/ticutility.pri)
include($$QTUTILITIES/translationutility/translationutility.pri)

include($$ROOT/testbedcore/testbedcore.pri)

RESOURCES *= $$APP_PATH/qml/qml.qrc
RESOURCES *= $$APP_PATH/configuration/configuration.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
