# SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

VERSION = 1.1.1

CONFIG *= C++17

QT += serialport

ROOT = $$absolute_path(../..)
ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTUTILITIES = $$absolute_path($$ACTLIBS/qtutilities)

SUPPORTED_LAN_KEYS = en fr

include($$ROOT/plugins/abstractpluginelements/abstractpluginelements.pri)

include($$ACTLIBS/qtlibs/import-qtseriallinklib.pri)

# Auto answer
## Models
HEADERS *= $$PLUGIN_PATH/autoanswer/models/autoansexpectedserialmsg.hpp
SOURCES *= $$PLUGIN_PATH/autoanswer/models/autoansexpectedserialmsg.cpp
HEADERS *= $$PLUGIN_PATH/autoanswer/models/autoansmsgtosend.hpp
SOURCES *= $$PLUGIN_PATH/autoanswer/models/autoansmsgtosend.cpp
HEADERS *= $$PLUGIN_PATH/autoanswer/models/autoanswerparams.hpp
SOURCES *= $$PLUGIN_PATH/autoanswer/models/autoanswerparams.cpp

## Modules
HEADERS *= $$PLUGIN_PATH/autoanswer/modules/startserialautoanswer.hpp
SOURCES *= $$PLUGIN_PATH/autoanswer/modules/startserialautoanswer.cpp
HEADERS *= $$PLUGIN_PATH/autoanswer/modules/stopserialautoanswer.hpp
SOURCES *= $$PLUGIN_PATH/autoanswer/modules/stopserialautoanswer.cpp

## Processes
HEADERS *= $$PLUGIN_PATH/autoanswer/processes/autoansserialhandler.hpp
SOURCES *= $$PLUGIN_PATH/autoanswer/processes/autoansserialhandler.cpp
HEADERS *= $$PLUGIN_PATH/autoanswer/processes/autoansseriallinkprocess.hpp
SOURCES *= $$PLUGIN_PATH/autoanswer/processes/autoansseriallinkprocess.cpp
HEADERS *= $$PLUGIN_PATH/autoanswer/processes/autoanswerseriallinkthread.hpp
SOURCES *= $$PLUGIN_PATH/autoanswer/processes/autoanswerseriallinkthread.cpp

# Plugin entry points
HEADERS *= $$PLUGIN_PATH/serialplugininterface.hpp
SOURCES *= $$PLUGIN_PATH/serialplugininterface.cpp

# Managers
HEADERS *= $$PLUGIN_PATH/managers/autoanswerserialmanager.hpp
SOURCES *= $$PLUGIN_PATH/managers/autoanswerserialmanager.cpp
HEADERS *= $$PLUGIN_PATH/managers/seriallinkbankglobalmanager.hpp
SOURCES *= $$PLUGIN_PATH/managers/seriallinkbankglobalmanager.cpp

# Models
HEADERS *= $$PLUGIN_PATH/models/serialdetectshareddata.hpp
SOURCES *= $$PLUGIN_PATH/models/serialdetectshareddata.cpp


# Call script sequence
HEADERS *= $$PLUGIN_PATH/script/comparemeasurefromscriptlogs.hpp
SOURCES *= $$PLUGIN_PATH/script/comparemeasurefromscriptlogs.cpp
HEADERS *= $$PLUGIN_PATH/script/detectfromscriptlogs.hpp
SOURCES *= $$PLUGIN_PATH/script/detectfromscriptlogs.cpp
HEADERS *= $$PLUGIN_PATH/script/oneargscriptcallviaseriallink.hpp
SOURCES *= $$PLUGIN_PATH/script/oneargscriptcallviaseriallink.cpp
HEADERS *= $$PLUGIN_PATH/script/simplescriptcallviaseriallink.hpp
SOURCES *= $$PLUGIN_PATH/script/simplescriptcallviaseriallink.cpp

# Sequence
HEADERS *= $$PLUGIN_PATH/sequence/seriallinkprobe.hpp
SOURCES *= $$PLUGIN_PATH/sequence/seriallinkprobe.cpp
HEADERS *= $$PLUGIN_PATH/sequence/seriallinksetup.hpp
SOURCES *= $$PLUGIN_PATH/sequence/seriallinksetup.cpp
HEADERS *= $$PLUGIN_PATH/sequence/seriallinkwriteread.hpp
SOURCES *= $$PLUGIN_PATH/sequence/seriallinkwriteread.cpp

include($$QTUTILITIES/collectionutility/collectionutility.pri)
include($$QTUTILITIES/statisticsutility/statisticsutility.pri)
include($$QTUTILITIES/stringutility/stringutility.pri)
include($$QTUTILITIES/threadutility/threadutility.pri)

include($$ROOT/plugins/multi-args-lib/multi-args-lib.pri)
include($$ROOT/plugins/serial-link-lib/serial-link-lib.pri)
include($$ROOT/plugins/hmi-helper-lib/hmi-helper-lib.pri)
include($$ROOT/plugins/detection-helper-lib/detection-helper-lib.pri)
