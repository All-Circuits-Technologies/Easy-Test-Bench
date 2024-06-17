# SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

ROOT = $$absolute_path(../../..)

# Need to be put before the abstractplugin info including
VERSION = 1.0

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/basicbankinterface.hpp
SOURCES *= $$PLUGIN_PATH/basicbankinterface.cpp
HEADERS *= $$PLUGIN_PATH/pluginsimpleinterface.hpp
SOURCES *= $$PLUGIN_PATH/pluginsimpleinterface.cpp
HEADERS *= $$PLUGIN_PATH/simplesequenceinstancemodule.hpp
SOURCES *= $$PLUGIN_PATH/simplesequenceinstancemodule.cpp
