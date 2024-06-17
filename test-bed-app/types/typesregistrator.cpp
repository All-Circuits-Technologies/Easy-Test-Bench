// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "typesregistrator.hpp"

#include "testbedcore/types/sequencestatetype.hpp"
#include "logsutility/logmsgtype.hpp"
#include "types/imagetype.hpp"
#include "types/pluginwidgettype.hpp"
#include "types/popupbtntype.hpp"

#include <QQmlEngine>


void TypesRegistrator::registerMetaTypes()
{
    ImageType::registerMetaTypes();
    PopUpBtnType::registerMetaTypes();
    PluginWidgetType::registerMetaTypes();
}

void TypesRegistrator::registerQmlTypes()
{
    ImageType::registerQmlTypes();
    PluginWidgetType::registerQmlTypes();

    qmlRegisterType<SequenceStateType>("app.types", 1, 0, "SequenceStateType");
    qmlRegisterType<LogMsgType>("app.types", 1, 0, "LogMsgType");
}
