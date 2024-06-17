# SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.1

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

HEADERS *= $$PLUGIN_PATH/src/aprojectcachemodule.hpp
SOURCES *= $$PLUGIN_PATH/src/aprojectcachemodule.cpp
HEADERS *= $$PLUGIN_PATH/src/getstringcachevaluemodule.hpp
SOURCES *= $$PLUGIN_PATH/src/getstringcachevaluemodule.cpp
HEADERS *= $$PLUGIN_PATH/src/setinputstringcachevaluemodule.hpp
SOURCES *= $$PLUGIN_PATH/src/setinputstringcachevaluemodule.cpp
HEADERS *= $$PLUGIN_PATH/src/testifcachevalueexistmodule.hpp
SOURCES *= $$PLUGIN_PATH/src/testifcachevalueexistmodule.cpp
