# SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.2

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Logging strategies
HEADERS *= $$PLUGIN_PATH/settings/areadsettingmodule.hpp
SOURCES *= $$PLUGIN_PATH/settings/areadsettingmodule.cpp
HEADERS *= $$PLUGIN_PATH/settings/readstrsettingmodule.hpp
SOURCES *= $$PLUGIN_PATH/settings/readstrsettingmodule.cpp
