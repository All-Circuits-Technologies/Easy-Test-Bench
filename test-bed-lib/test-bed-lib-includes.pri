# SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

!defined(ROOT, var) : error($${_PRO_FILE_} requires to have ROOT variable)
!defined(TESTBEDLIB_ROOT, var) : error($${_PRO_FILE_} requires to have TESTBEDLIB_ROOT variable)

DEFINES += TESTBEDLIB_LIBRARY

ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTUTILITIES = $$absolute_path($$ACTLIBS/qtutilities)

SUPPORTED_LAN_KEYS = en fr

INCLUDEPATH *= $$TESTBEDLIB_ROOT
INCLUDEPATH *= $$ROOT
INCLUDEPATH *= $$QTUTILITIES

# Bank elements
SOURCES *= $$TESTBEDLIB_ROOT/bank/adisplayuserinterface.cpp
HEADERS *= $$TESTBEDLIB_ROOT/bank/adisplayuserinterface.hpp
SOURCES *= $$TESTBEDLIB_ROOT/bank/bankcommoninterface.cpp
HEADERS *= $$TESTBEDLIB_ROOT/bank/bankcommoninterface.hpp
SOURCES *= $$TESTBEDLIB_ROOT/bank/bankmanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/bank/bankmanager.hpp
SOURCES *= $$TESTBEDLIB_ROOT/bank/displayuitoken.cpp
HEADERS *= $$TESTBEDLIB_ROOT/bank/displayuitoken.hpp

# Core, contains all the shared and generic elements
HEADERS *= $$TESTBEDLIB_ROOT/core/coreconstants.hpp
SOURCES *= $$TESTBEDLIB_ROOT/core/globalmanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/core/globalmanager.hpp
HEADERS *= $$TESTBEDLIB_ROOT/core/knownsettingdetails.hpp
SOURCES *= $$TESTBEDLIB_ROOT/core/knownsettings.cpp
HEADERS *= $$TESTBEDLIB_ROOT/core/knownsettings.hpp
SOURCES *= $$TESTBEDLIB_ROOT/core/settingsmanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/core/settingsmanager.hpp

# Database utilities
SOURCES *= $$TESTBEDLIB_ROOT/database/databaseauthinfo.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/databaseauthinfo.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/dbexecuter.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/dbexecuter.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/dbliboptionsasker.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/dbliboptionsasker.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/dbmanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/dbmanager.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/dbpermissionasker.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/dbpermissionasker.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/dbpluginpermstorageasker.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/dbpluginpermstorageasker.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/dbprojectasker.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/dbprojectasker.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/dbthread.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/dbthread.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/dbuserasker.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/dbuserasker.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/handler/dbhandler.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/handler/dbhandler.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/handler/dbhandlerkey.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/handler/dbhandlerkey.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/handler/dbhandlermanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/handler/dbhandlermanager.hpp
SOURCES *= $$TESTBEDLIB_ROOT/database/handler/dbsqlqueryhandler.cpp
HEADERS *= $$TESTBEDLIB_ROOT/database/handler/dbsqlqueryhandler.hpp

# Database models
SOURCES *= $$TESTBEDLIB_ROOT/models/dbobject.cpp
HEADERS *= $$TESTBEDLIB_ROOT/models/dbobject.hpp
SOURCES *= $$TESTBEDLIB_ROOT/models/liboptions.cpp
HEADERS *= $$TESTBEDLIB_ROOT/models/liboptions.hpp
SOURCES *= $$TESTBEDLIB_ROOT/models/permission.cpp
HEADERS *= $$TESTBEDLIB_ROOT/models/permission.hpp
SOURCES *= $$TESTBEDLIB_ROOT/models/pluginpermstoragevalue.cpp
HEADERS *= $$TESTBEDLIB_ROOT/models/pluginpermstoragevalue.hpp
SOURCES *= $$TESTBEDLIB_ROOT/models/project.cpp
HEADERS *= $$TESTBEDLIB_ROOT/models/project.hpp
SOURCES *= $$TESTBEDLIB_ROOT/models/role.cpp
HEADERS *= $$TESTBEDLIB_ROOT/models/role.hpp
SOURCES *= $$TESTBEDLIB_ROOT/models/user.cpp
HEADERS *= $$TESTBEDLIB_ROOT/models/user.hpp

# Permission utilities
SOURCES *= $$TESTBEDLIB_ROOT/permission/authenticationmanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/permission/authenticationmanager.hpp
SOURCES *= $$TESTBEDLIB_ROOT/permission/loadingstatus.cpp
HEADERS *= $$TESTBEDLIB_ROOT/permission/loadingstatus.hpp
SOURCES *= $$TESTBEDLIB_ROOT/permission/loaduserpermissionsthread.cpp
HEADERS *= $$TESTBEDLIB_ROOT/permission/loaduserpermissionsthread.hpp
SOURCES *= $$TESTBEDLIB_ROOT/permission/permissionmanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/permission/permissionmanager.hpp

# Errors
SOURCES *= $$TESTBEDLIB_ROOT/errors/sequenceerror.cpp
HEADERS *= $$TESTBEDLIB_ROOT/errors/sequenceerror.hpp
SOURCES *= $$TESTBEDLIB_ROOT/errors/sequenceloadingerrorhandler.cpp
HEADERS *= $$TESTBEDLIB_ROOT/errors/sequenceloadingerrorhandler.hpp
#Helpers
SOURCES *= $$TESTBEDLIB_ROOT/errors/helpers/sequenceloadingerrorhelper.cpp
HEADERS *= $$TESTBEDLIB_ROOT/errors/helpers/sequenceloadingerrorhelper.hpp

# File
SOURCES *= $$TESTBEDLIB_ROOT/file/ajsonfile.cpp
HEADERS *= $$TESTBEDLIB_ROOT/file/ajsonfile.hpp

# Json parsers
SOURCES *= $$TESTBEDLIB_ROOT/jsonparsers/ajsonparser.cpp
HEADERS *= $$TESTBEDLIB_ROOT/jsonparsers/ajsonparser.hpp
SOURCES *= $$TESTBEDLIB_ROOT/jsonparsers/bmsgadgetjsonloader.cpp
HEADERS *= $$TESTBEDLIB_ROOT/jsonparsers/bmsgadgetjsonloader.hpp
SOURCES *= $$TESTBEDLIB_ROOT/jsonparsers/databaseinfosjsonparser.cpp
HEADERS *= $$TESTBEDLIB_ROOT/jsonparsers/databaseinfosjsonparser.hpp
SOURCES *= $$TESTBEDLIB_ROOT/jsonparsers/jsontonodesparser.cpp
HEADERS *= $$TESTBEDLIB_ROOT/jsonparsers/jsontonodesparser.hpp
SOURCES *= $$TESTBEDLIB_ROOT/jsonparsers/nodestojsonparser.cpp
HEADERS *= $$TESTBEDLIB_ROOT/jsonparsers/nodestojsonparser.hpp
SOURCES *= $$TESTBEDLIB_ROOT/jsonparsers/projectfilejsonparser.cpp
HEADERS *= $$TESTBEDLIB_ROOT/jsonparsers/projectfilejsonparser.hpp
SOURCES *= $$TESTBEDLIB_ROOT/jsonparsers/sequencefilejsonparser.cpp
HEADERS *= $$TESTBEDLIB_ROOT/jsonparsers/sequencefilejsonparser.hpp
SOURCES *= $$TESTBEDLIB_ROOT/jsonparsers/sequencejsonarrayloader.cpp
HEADERS *= $$TESTBEDLIB_ROOT/jsonparsers/sequencejsonarrayloader.hpp
SOURCES *= $$TESTBEDLIB_ROOT/jsonparsers/topologyfilejsonparser.cpp
HEADERS *= $$TESTBEDLIB_ROOT/jsonparsers/topologyfilejsonparser.hpp

# Project components
SOURCES *= $$TESTBEDLIB_ROOT/project/projectcache.cpp
HEADERS *= $$TESTBEDLIB_ROOT/project/projectcache.hpp
SOURCES *= $$TESTBEDLIB_ROOT/project/projectfile.cpp
HEADERS *= $$TESTBEDLIB_ROOT/project/projectfile.hpp
SOURCES *= $$TESTBEDLIB_ROOT/project/projectmanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/project/projectmanager.hpp

# Sequence components

#   # Access key
HEADERS *= $$TESTBEDLIB_ROOT/sequence/accesskeys/asequencecontainernodekey.hpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/accesskeys/asequencenodekey.hpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/accesskeys/forknodekey.hpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/accesskeys/inclusionnodekey.hpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/accesskeys/joinnodekey.hpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/accesskeys/nodessequencekey.hpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/accesskeys/targetingsequencenodeskey.hpp

#   # Core state machine
SOURCES *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/anodeport.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/anodeport.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/innodeport.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/innodeport.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/nodetype.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/nodetype.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/outnodeport.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/outnodeport.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/referencesholder.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/referencesholder.hpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/referencesholdertype.hpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/corestatemachine/targettype.hpp

#   # Decisional transition
SOURCES *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/adecisionaltransition.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/adecisionaltransition.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/alogicaloperatordecisionaltransition.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/alogicaloperatordecisionaltransition.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/anddecisionaltransition.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/anddecisionaltransition.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/decisionaleventvalue.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/decisionaleventvalue.hpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/decisionaloperator.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/equalitydecisionaltransition.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/equalitydecisionaltransition.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/greaterthandecisionaltransition.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/greaterthandecisionaltransition.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/lesserthandecisionaltransition.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/lesserthandecisionaltransition.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/ordecisionaltransition.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/decisionaltransition/ordecisionaltransition.hpp

#   # Logs
SOURCES *= $$TESTBEDLIB_ROOT/sequence/logs/sequencelogmanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/logs/sequencelogmanager.hpp

#   # Nodes
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/anode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/anode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/asequencecontainernode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/asequencecontainernode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/asequencenode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/asequencenode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/atargetingnode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/atargetingnode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/decisionalnode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/decisionalnode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/finalnode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/finalnode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/forknode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/forknode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/groupnode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/groupnode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/inclusionnode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/inclusionnode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/initialnode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/initialnode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/joinnode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/joinnode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/mergenode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/mergenode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodes/sequencemodulenode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodes/sequencemodulenode.hpp

#   # Reference nodes
SOURCES *= $$TESTBEDLIB_ROOT/sequence/referencenodes/areferencenode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/referencenodes/areferencenode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/referencenodes/constantnode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/referencenodes/constantnode.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/referencenodes/variablenode.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/referencenodes/variablenode.hpp

#   # Main elements
SOURCES *= $$TESTBEDLIB_ROOT/sequence/checksequencevalidity.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/checksequencevalidity.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodessequence.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodessequence.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/nodessequencemanager.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/nodessequencemanager.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/sequencefile.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/sequencefile.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/sequencenodeshelper.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/sequencenodeshelper.hpp
SOURCES *= $$TESTBEDLIB_ROOT/sequence/sequencethread.cpp
HEADERS *= $$TESTBEDLIB_ROOT/sequence/sequencethread.hpp

# Topology components
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfile.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfile.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfilebinding.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfilebinding.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfilebindingpin.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfilebindingpin.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfilebindings.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfilebindings.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfilecomponent.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfilecomponent.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfilecomponentpin.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfilecomponentpin.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfilecomponentpingroup.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfilecomponentpingroup.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfileinstance.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfileinstance.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfileinstanceparam.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfileinstanceparam.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfileinstances.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfileinstances.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfilelibrary.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfilelibrary.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfileroute.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfileroute.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfileroutepoint.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfileroutepoint.hpp
SOURCES *= $$TESTBEDLIB_ROOT/topology/topologyfileroutes.cpp
HEADERS *= $$TESTBEDLIB_ROOT/topology/topologyfileroutes.hpp

# Tools
SOURCES *= $$TESTBEDLIB_ROOT/tools/qt/bmsgadget.cpp
HEADERS *= $$TESTBEDLIB_ROOT/tools/qt/bmsgadget.hpp
SOURCES *= $$TESTBEDLIB_ROOT/tools/qt/qobjecthelper.cpp
HEADERS *= $$TESTBEDLIB_ROOT/tools/qt/qobjecthelper.hpp

include($$QTUTILITIES/definesutility/definesutility.pri)
include($$QTUTILITIES/translationutility/translationutility.pri)
include($$QTUTILITIES/jsonutility/jsonutility.pri)
include($$QTUTILITIES/waitutility/waitutility.pri)
include($$QTUTILITIES/collectionutility/collectionutility.pri)
include($$QTUTILITIES/threadutility/threadutility.pri)
include($$QTUTILITIES/fileutility/fileutility.pri)
include($$QTUTILITIES/processutility/processutility.pri)
include($$QTUTILITIES/logsutility/logsutility.pri)
include($$QTUTILITIES/byteutility/byteutility.pri)
include($$QTUTILITIES/numberutility/numberutility.pri)
include($$QTUTILITIES/managersutility/managersutility.pri)
include($$QTUTILITIES/yamlutility/yamlutility.pri)
include($$QTUTILITIES/ticutility/ticutility.pri)
include($$QTUTILITIES/statemachineutility/statemachineutility.pri)

include($$ROOT/examples/examples.pri)
include($$ROOT/testbedcore/testbedcore.pri)
include($$ROOT/bankcoreshared/bankcoreshared.pri)
