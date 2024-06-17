# SPDX-FileCopyrightText: 2021 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.0.1

ROOT = $$absolute_path(../..)
ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTUTILITIES = $$absolute_path($$ACTLIBS/qtutilities)
QTLIBS = $$absolute_path($$ACTLIBS/qtlibs)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)
include($$QTLIBS/import-qtvisacomlib.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp
HEADERS *= $$PLUGIN_PATH/idncmd.hpp
SOURCES *= $$PLUGIN_PATH/idncmd.cpp

# Operations
HEADERS *= $$PLUGIN_PATH/pulse/apulse.hpp
SOURCES *= $$PLUGIN_PATH/pulse/apulse.cpp
HEADERS *= $$PLUGIN_PATH/pulse/pulse4.hpp
SOURCES *= $$PLUGIN_PATH/pulse/pulse4.cpp
HEADERS *= $$PLUGIN_PATH/pulse/pulse4_iso.hpp
SOURCES *= $$PLUGIN_PATH/pulse/pulse4_iso.cpp
HEADERS *= $$PLUGIN_PATH/pulse/pulse4_gm.hpp
SOURCES *= $$PLUGIN_PATH/pulse/pulse4_gm.cpp
HEADERS *= $$PLUGIN_PATH/pulse/pulseflag.hpp
SOURCES *= $$PLUGIN_PATH/pulse/pulseflag.cpp
HEADERS *= $$PLUGIN_PATH/pulse/triggerenum.hpp
SOURCES *= $$PLUGIN_PATH/pulse/triggerenum.cpp
HEADERS *= $$PLUGIN_PATH/pulse/voltageinterruption.hpp
SOURCES *= $$PLUGIN_PATH/pulse/voltageinterruption.cpp

HEADERS *= $$PLUGIN_PATH/pulsecmd/apulsecmd.hpp
SOURCES *= $$PLUGIN_PATH/pulsecmd/apulsecmd.cpp
HEADERS *= $$PLUGIN_PATH/pulsecmd/pulsecmd.hpp
SOURCES *= $$PLUGIN_PATH/pulsecmd/pulsecmd.cpp
HEADERS *= $$PLUGIN_PATH/pulsecmd/pulseenum.hpp
SOURCES *= $$PLUGIN_PATH/pulsecmd/pulseenum.cpp

include($$ROOT/plugins/emtestbenchutility/emtestbenchutility.pri)
include($$QTUTILITIES/byteutility/byteutility.pri)
include($$QTUTILITIES/intelhexfileutility/intelhexfileutility.pri)
