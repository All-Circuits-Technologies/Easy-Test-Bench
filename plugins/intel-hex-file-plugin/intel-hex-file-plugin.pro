# SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.1

ROOT = $$absolute_path(../..)
QTUTILITIES = $$absolute_path(../qtutilities)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Functions
HEADERS *= $$PLUGIN_PATH/functions/aupdatehexfile.hpp
SOURCES *= $$PLUGIN_PATH/functions/aupdatehexfile.cpp
HEADERS *= $$PLUGIN_PATH/functions/updatehexfile.hpp
SOURCES *= $$PLUGIN_PATH/functions/updatehexfile.cpp

include($$QTUTILITIES/byteutility/byteutility.pri)
include($$QTUTILITIES/intelhexfileutility/intelhexfileutility.pri)
