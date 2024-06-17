# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.2
QT += serialport

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Sequence
HEADERS *= $$PLUGIN_PATH/sequence/analyseresults.hpp
SOURCES *= $$PLUGIN_PATH/sequence/analyseresults.cpp
HEADERS *= $$PLUGIN_PATH/sequence/displayerrorresults.hpp
SOURCES *= $$PLUGIN_PATH/sequence/displayerrorresults.cpp

include($$ROOT/plugins/hmi-helper-lib/hmi-helper-lib.pri)
