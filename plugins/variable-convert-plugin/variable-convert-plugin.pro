# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.1

ROOT = $$absolute_path(../..)
QTUTILITIES = $$absolute_path($$ROOT/qtutilities)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

HEADERS *= $$PLUGIN_PATH/plugininterface.hpp
SOURCES *= $$PLUGIN_PATH/plugininterface.cpp

# Operations
HEADERS *= $$PLUGIN_PATH/operations/endianesstype.hpp
SOURCES *= $$PLUGIN_PATH/operations/endianesstype.cpp
HEADERS *= $$PLUGIN_PATH/operations/numbertobytearrayconverter.hpp
SOURCES *= $$PLUGIN_PATH/operations/numbertobytearrayconverter.cpp

include($$QTUTILITIES/byteutility/byteutility.pri)
