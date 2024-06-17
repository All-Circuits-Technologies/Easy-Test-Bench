# SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.2

ROOT = $$absolute_path(../..)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/bankglobalmanager.hpp
SOURCES *= $$PLUGIN_PATH/bankglobalmanager.cpp
HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

HEADERS *= $$PLUGIN_PATH/functions/awaitmodule.hpp
SOURCES *= $$PLUGIN_PATH/functions/awaitmodule.cpp
HEADERS *= $$PLUGIN_PATH/functions/paramentrywaitmodule.hpp
SOURCES *= $$PLUGIN_PATH/functions/paramentrywaitmodule.cpp
HEADERS *= $$PLUGIN_PATH/functions/refentrywaitmodule.hpp
SOURCES *= $$PLUGIN_PATH/functions/refentrywaitmodule.cpp

HEADERS *= $$PLUGIN_PATH/functions/elapsedtimer/elapsedtimermanager.hpp
SOURCES *= $$PLUGIN_PATH/functions/elapsedtimer/elapsedtimermanager.cpp
HEADERS *= $$PLUGIN_PATH/functions/elapsedtimer/fireelapsedtimermodule.hpp
SOURCES *= $$PLUGIN_PATH/functions/elapsedtimer/fireelapsedtimermodule.cpp
HEADERS *= $$PLUGIN_PATH/functions/elapsedtimer/waitforanelapsedtimermodule.hpp
SOURCES *= $$PLUGIN_PATH/functions/elapsedtimer/waitforanelapsedtimermodule.cpp
