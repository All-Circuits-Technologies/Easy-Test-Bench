# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.1

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Logging strategies
HEADERS *= $$PLUGIN_PATH/src/afilebrowsermodule.hpp
SOURCES *= $$PLUGIN_PATH/src/afilebrowsermodule.cpp
HEADERS *= $$PLUGIN_PATH/src/filebrowermodule.hpp
SOURCES *= $$PLUGIN_PATH/src/filebrowermodule.cpp
HEADERS *= $$PLUGIN_PATH/src/permstorefilebrowsermodule.hpp
SOURCES *= $$PLUGIN_PATH/src/permstorefilebrowsermodule.cpp
HEADERS *= $$PLUGIN_PATH/src/projectcachebrowsermodule.hpp
SOURCES *= $$PLUGIN_PATH/src/projectcachebrowsermodule.cpp

include($$ROOT/plugins/hmi-helper-lib/hmi-helper-lib.pri)
