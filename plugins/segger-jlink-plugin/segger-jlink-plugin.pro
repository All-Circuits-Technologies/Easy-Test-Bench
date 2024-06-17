# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2020 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.1

CONFIG *= C++17

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Modules
HEADERS *= $$PLUGIN_PATH/functions/ajlinkinterface.hpp
SOURCES *= $$PLUGIN_PATH/functions/ajlinkinterface.cpp
HEADERS *= $$PLUGIN_PATH/functions/ajlinkintffromfile.hpp
SOURCES *= $$PLUGIN_PATH/functions/ajlinkintffromfile.cpp
HEADERS *= $$PLUGIN_PATH/functions/ajlinkintffromparams.hpp
SOURCES *= $$PLUGIN_PATH/functions/ajlinkintffromparams.cpp
HEADERS *= $$PLUGIN_PATH/functions/detectboard.hpp
SOURCES *= $$PLUGIN_PATH/functions/detectboard.cpp
HEADERS *= $$PLUGIN_PATH/functions/jlinkerase.hpp
SOURCES *= $$PLUGIN_PATH/functions/jlinkerase.cpp
HEADERS *= $$PLUGIN_PATH/functions/jlinkflash.hpp
SOURCES *= $$PLUGIN_PATH/functions/jlinkflash.cpp
HEADERS *= $$PLUGIN_PATH/functions/jlinkfromfileonearg.hpp
SOURCES *= $$PLUGIN_PATH/functions/jlinkfromfileonearg.cpp
HEADERS *= $$PLUGIN_PATH/functions/jlinkreset.hpp
SOURCES *= $$PLUGIN_PATH/functions/jlinkreset.cpp

# Models
HEADERS *= $$PLUGIN_PATH/models/detectjlinkshareddata.hpp
SOURCES *= $$PLUGIN_PATH/models/detectjlinkshareddata.cpp

include($$ROOT/plugins/multi-args-lib/multi-args-lib.pri)
include($$ROOT/plugins/hmi-helper-lib/hmi-helper-lib.pri)
include($$ROOT/plugins/detection-helper-lib/detection-helper-lib.pri)
