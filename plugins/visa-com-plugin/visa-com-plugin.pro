# SPDX-FileCopyrightText: 2021 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.1

ROOT = $$absolute_path(../..)
ACTLIBS = $$absolute_path($$ROOT/actlibs)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)
include($$ACTLIBS/qtlibs/import-qtvisacomlib.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Operations
HEADERS *= $$PLUGIN_PATH/setup/avisasetup.hpp
SOURCES *= $$PLUGIN_PATH/setup/avisasetup.cpp
HEADERS *= $$PLUGIN_PATH/setup/gpibvisasetup.hpp
SOURCES *= $$PLUGIN_PATH/setup/gpibvisasetup.cpp
HEADERS *= $$PLUGIN_PATH/setup/gpib4881visasetup.hpp
SOURCES *= $$PLUGIN_PATH/setup/gpib4881visasetup.cpp
HEADERS *= $$PLUGIN_PATH/setup/serialvisasetup.hpp
SOURCES *= $$PLUGIN_PATH/setup/serialvisasetup.cpp
HEADERS *= $$PLUGIN_PATH/setup/usbvisasetup.hpp
SOURCES *= $$PLUGIN_PATH/setup/usbvisasetup.cpp
