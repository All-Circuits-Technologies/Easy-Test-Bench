# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.1

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Operations
HEADERS *= $$PLUGIN_PATH/operations/abaseoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/abaseoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/afromrefoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/afromrefoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/avalueassignmentoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/avalueassignmentoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/frommultirefnumberarithmeticoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/frommultirefnumberarithmeticoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/fromrefboolassignmentoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/fromrefboolassignmentoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/fromrefnumberarithmeticoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/fromrefnumberarithmeticoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/fromrefnumberassignmentoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/fromrefnumberassignmentoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/fromrefstringassignmentoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/fromrefstringassignmentoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/valueboolassignmentoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/valueboolassignmentoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/valuenumberassignmentoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/valuenumberassignmentoperator.cpp
HEADERS *= $$PLUGIN_PATH/operations/valuestringassignmentoperator.hpp
SOURCES *= $$PLUGIN_PATH/operations/valuestringassignmentoperator.cpp
