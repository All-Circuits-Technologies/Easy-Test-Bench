# SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.1

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Functions
HEADERS *= $$PLUGIN_PATH/functions/aatmelprogramitf.hpp
SOURCES *= $$PLUGIN_PATH/functions/aatmelprogramitf.cpp
HEADERS *= $$PLUGIN_PATH/functions/detectboardatprogram.hpp
SOURCES *= $$PLUGIN_PATH/functions/detectboardatprogram.cpp
HEADERS *= $$PLUGIN_PATH/functions/programatprogram.hpp
SOURCES *= $$PLUGIN_PATH/functions/programatprogram.cpp
HEADERS *= $$PLUGIN_PATH/functions/readeepromatprogram.hpp
SOURCES *= $$PLUGIN_PATH/functions/readeepromatprogram.cpp

# Models
HEADERS *= $$PLUGIN_PATH/models/detectatmelshareddata.hpp
SOURCES *= $$PLUGIN_PATH/models/detectatmelshareddata.cpp

# Types
HEADERS *= $$PLUGIN_PATH/types/atprogramcmds.hpp
SOURCES *= $$PLUGIN_PATH/types/atprogramcmds.cpp
HEADERS *= $$PLUGIN_PATH/types/atprogramphysicalitftype.hpp
SOURCES *= $$PLUGIN_PATH/types/atprogramphysicalitftype.cpp
HEADERS *= $$PLUGIN_PATH/types/memorytype.hpp
SOURCES *= $$PLUGIN_PATH/types/memorytype.cpp

include($$ROOT/plugins/hmi-helper-lib/hmi-helper-lib.pri)
include($$ROOT/plugins/detection-helper-lib/detection-helper-lib.pri)
