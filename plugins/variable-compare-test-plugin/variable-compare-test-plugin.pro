# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.2

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Compare classes
HEADERS *= $$PLUGIN_PATH/comparators/abasecomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/abasecomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/arefandparamcomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/arefandparamcomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/atworefscomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/atworefscomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/refandparamboolcomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/refandparamboolcomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/refandparamnumbercomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/refandparamnumbercomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/refandparamstringcomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/refandparamstringcomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/tworefsboolarraycomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/tworefsboolarraycomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/tworefsboolcomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/tworefsboolcomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/tworefsnumberarraycomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/tworefsnumberarraycomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/tworefsnumbercomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/tworefsnumbercomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/tworefsstringarraycomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/tworefsstringarraycomparator.cpp
HEADERS *= $$PLUGIN_PATH/comparators/tworefsstringcomparator.hpp
SOURCES *= $$PLUGIN_PATH/comparators/tworefsstringcomparator.cpp
