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

# CSV
HEADERS *= $$PLUGIN_PATH/csv/acsvadd.hpp
SOURCES *= $$PLUGIN_PATH/csv/acsvadd.cpp
HEADERS *= $$PLUGIN_PATH/csv/csvaddnumber.hpp
SOURCES *= $$PLUGIN_PATH/csv/csvaddnumber.cpp
HEADERS *= $$PLUGIN_PATH/csv/csvaddstring.hpp
SOURCES *= $$PLUGIN_PATH/csv/csvaddstring.cpp
HEADERS *= $$PLUGIN_PATH/csv/csvaddts.hpp
SOURCES *= $$PLUGIN_PATH/csv/csvaddts.cpp
HEADERS *= $$PLUGIN_PATH/csv/csvlogline.hpp
SOURCES *= $$PLUGIN_PATH/csv/csvlogline.cpp

# Logging strategies
HEADERS *= $$PLUGIN_PATH/loggers/asavinglogfile.hpp
SOURCES *= $$PLUGIN_PATH/loggers/asavinglogfile.cpp
HEADERS *= $$PLUGIN_PATH/loggers/logmsgbyboolcondition.hpp
SOURCES *= $$PLUGIN_PATH/loggers/logmsgbyboolcondition.cpp
HEADERS *= $$PLUGIN_PATH/loggers/oneboardonefile.hpp
SOURCES *= $$PLUGIN_PATH/loggers/oneboardonefile.cpp
HEADERS *= $$PLUGIN_PATH/loggers/onlyonefile.hpp
SOURCES *= $$PLUGIN_PATH/loggers/onlyonefile.cpp
