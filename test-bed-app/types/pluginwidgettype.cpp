// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "pluginwidgettype.hpp"

#include "testbedcore/parsers/jsonparsersconstants.hpp"

#include <QQmlEngine>


PluginWidgetType::Enum PluginWidgetType::parseFromString(const QString &objectType)
{
    Enum pluginType = Unknown;

    if(objectType == JsonParsersConstants::Hmi::RadioListButtonsTypeAttribute)
    {
        pluginType = RadioListButtons;
    }
    else if(objectType == JsonParsersConstants::Hmi::StringTypeAttribute)
    {
        pluginType = String;
    }
    else if(objectType == JsonParsersConstants::Hmi::SelectFileTypeAttribute)
    {
        pluginType = SelectFile;
    }

    return pluginType;
}

void PluginWidgetType::registerMetaTypes()
{
    qRegisterMetaType<PluginWidgetType::Enum>("PluginWidgetType::Enum");
}

void PluginWidgetType::registerQmlTypes()
{
    qmlRegisterType<PluginWidgetType>("app.types", 1, 0, "PluginWidgetType");
}
