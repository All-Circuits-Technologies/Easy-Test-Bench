# SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.0.1
QT += serialbus

CONFIG += c++17

ROOT = $$absolute_path(../..)
ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTUTILITIES = $$absolute_path($$ACTLIBS/qtutilities)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

include($$ACTLIBS/qtlibs/import-qtpeakcanlib.pri)

# Plugin entry points
HEADERS *= $$PLUGIN_PATH/peakcanbankglobalmanager.hpp
SOURCES *= $$PLUGIN_PATH/peakcanbankglobalmanager.cpp
HEADERS *= $$PLUGIN_PATH/peakcanconstants.hpp
HEADERS *= $$PLUGIN_PATH/peakcanplugininterface.hpp
SOURCES *= $$PLUGIN_PATH/peakcanplugininterface.cpp

# Managers
HEADERS *= $$PLUGIN_PATH/managers/autoanswerpeakcanmanager.hpp
SOURCES *= $$PLUGIN_PATH/managers/autoanswerpeakcanmanager.cpp

# Models

## Global
HEADERS *= $$PLUGIN_PATH/models/apcanexpectedmsg.hpp
SOURCES *= $$PLUGIN_PATH/models/apcanexpectedmsg.cpp
HEADERS *= $$PLUGIN_PATH/models/apcanseqactioninfo.hpp
SOURCES *= $$PLUGIN_PATH/models/apcanseqactioninfo.cpp
HEADERS *= $$PLUGIN_PATH/models/canmsgseqparams.hpp
SOURCES *= $$PLUGIN_PATH/models/canmsgseqparams.cpp

## Automatic answer
HEADERS *= $$PLUGIN_PATH/models/autoanswer/autoansexpectedmessage.hpp
SOURCES *= $$PLUGIN_PATH/models/autoanswer/autoansexpectedmessage.cpp
HEADERS *= $$PLUGIN_PATH/models/autoanswer/autoanssequactioninfo.hpp
SOURCES *= $$PLUGIN_PATH/models/autoanswer/autoanssequactioninfo.cpp
HEADERS *= $$PLUGIN_PATH/models/autoanswer/autoanssequenceinfo.hpp
SOURCES *= $$PLUGIN_PATH/models/autoanswer/autoanssequenceinfo.cpp

## Read and/or Write module
HEADERS *= $$PLUGIN_PATH/models/readandwrite/pcanexpectedmessage.hpp
SOURCES *= $$PLUGIN_PATH/models/readandwrite/pcanexpectedmessage.cpp
HEADERS *= $$PLUGIN_PATH/models/readandwrite/pcansequenceactioninfo.hpp
SOURCES *= $$PLUGIN_PATH/models/readandwrite/pcansequenceactioninfo.cpp
HEADERS *= $$PLUGIN_PATH/models/readandwrite/pcansequenceinfo.hpp
SOURCES *= $$PLUGIN_PATH/models/readandwrite/pcansequenceinfo.cpp

## Compare module
HEADERS *= $$PLUGIN_PATH/models/comparemsg/compareexpectedmsg.hpp
SOURCES *= $$PLUGIN_PATH/models/comparemsg/compareexpectedmsg.cpp
HEADERS *= $$PLUGIN_PATH/models/comparemsg/comparecansequenceactioninfo.hpp
SOURCES *= $$PLUGIN_PATH/models/comparemsg/comparecansequenceactioninfo.cpp
HEADERS *= $$PLUGIN_PATH/models/comparemsg/comparecansequenceinfo.hpp
SOURCES *= $$PLUGIN_PATH/models/comparemsg/comparecansequenceinfo.cpp

# Modules

## Automatic answer
HEADERS *= $$PLUGIN_PATH/modules/autoanswer/startautoanswer.hpp
SOURCES *= $$PLUGIN_PATH/modules/autoanswer/startautoanswer.cpp
HEADERS *= $$PLUGIN_PATH/modules/autoanswer/stopautoanswer.hpp
SOURCES *= $$PLUGIN_PATH/modules/autoanswer/stopautoanswer.cpp

## Read and/or Write module
HEADERS *= $$PLUGIN_PATH/modules/readandwrite/acanmsgsequence.hpp
SOURCES *= $$PLUGIN_PATH/modules/readandwrite/acanmsgsequence.cpp
HEADERS *= $$PLUGIN_PATH/modules/readandwrite/acanwriting.hpp
SOURCES *= $$PLUGIN_PATH/modules/readandwrite/acanwriting.cpp
HEADERS *= $$PLUGIN_PATH/modules/readandwrite/multipleonlyreadcanmsgs.hpp
SOURCES *= $$PLUGIN_PATH/modules/readandwrite/multipleonlyreadcanmsgs.cpp
HEADERS *= $$PLUGIN_PATH/modules/readandwrite/multiplewritecanmsgs.hpp
SOURCES *= $$PLUGIN_PATH/modules/readandwrite/multiplewritecanmsgs.cpp
HEADERS *= $$PLUGIN_PATH/modules/readandwrite/onlyreadcanmsg.hpp
SOURCES *= $$PLUGIN_PATH/modules/readandwrite/onlyreadcanmsg.cpp
HEADERS *= $$PLUGIN_PATH/modules/readandwrite/writecanmsg.hpp
SOURCES *= $$PLUGIN_PATH/modules/readandwrite/writecanmsg.cpp

## Compare module
HEADERS *= $$PLUGIN_PATH/modules/comparemsg/comparecanmsg.hpp
SOURCES *= $$PLUGIN_PATH/modules/comparemsg/comparecanmsg.cpp

# Processes
HEADERS *= $$PLUGIN_PATH/process/autoansprocessthread.hpp
SOURCES *= $$PLUGIN_PATH/process/autoansprocessthread.cpp
HEADERS *= $$PLUGIN_PATH/process/autoansthreadhandler.hpp
SOURCES *= $$PLUGIN_PATH/process/autoansthreadhandler.cpp
HEADERS *= $$PLUGIN_PATH/process/autoanswerprocess.hpp
SOURCES *= $$PLUGIN_PATH/process/autoanswerprocess.cpp
HEADERS *= $$PLUGIN_PATH/process/canmsgprocess.hpp
SOURCES *= $$PLUGIN_PATH/process/canmsgprocess.cpp
HEADERS *= $$PLUGIN_PATH/process/comparemsgprocess.hpp
SOURCES *= $$PLUGIN_PATH/process/comparemsgprocess.cpp

# Sequence
HEADERS *= $$PLUGIN_PATH/setup/acansetup.hpp
SOURCES *= $$PLUGIN_PATH/setup/acansetup.cpp
HEADERS *= $$PLUGIN_PATH/setup/canfdsetup.hpp
SOURCES *= $$PLUGIN_PATH/setup/canfdsetup.cpp
HEADERS *= $$PLUGIN_PATH/setup/cansetup.hpp
SOURCES *= $$PLUGIN_PATH/setup/cansetup.cpp

include($$QTUTILITIES/byteutility/byteutility.pri)
include($$QTUTILITIES/canutility/canutility.pri)
include($$QTUTILITIES/handlerutility/handlerutility.pri)
include($$QTUTILITIES/collectionutility/collectionutility.pri)
include($$QTUTILITIES/statisticsutility/statisticsutility.pri)
include($$QTUTILITIES/stringutility/stringutility.pri)

include($$ROOT/plugins/peak-can-lib/peak-can-lib.pri)
