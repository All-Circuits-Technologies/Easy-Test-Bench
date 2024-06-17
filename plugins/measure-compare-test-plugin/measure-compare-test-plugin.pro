# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.2

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Compare classes
HEADERS *= $$PLUGIN_PATH/comparators/ameasureboundstest.hpp
SOURCES *= $$PLUGIN_PATH/comparators/ameasureboundstest.cpp
HEADERS *= $$PLUGIN_PATH/comparators/measureinputsboundstest.hpp
SOURCES *= $$PLUGIN_PATH/comparators/measureinputsboundstest.cpp
HEADERS *= $$PLUGIN_PATH/comparators/measureparamsboundstest.hpp
SOURCES *= $$PLUGIN_PATH/comparators/measureparamsboundstest.cpp
