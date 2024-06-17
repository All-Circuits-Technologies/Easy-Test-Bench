// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "projectcachebrowsermodule.hpp"

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


ProjectCacheBrowserModule::ProjectCacheBrowserModule(QObject *parent) :
    AFileBrowserModule(staticMetaObject.className(), parent)
{
}

bool ProjectCacheBrowserModule::initInputsFormat(JsonArray &inputsFormat) const
{
    JsonObject projectCacheKey;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputProjectCacheKeyName,
                                                         projectCacheKey,
                                                         InputProjectCacheKeyDesc));

    inputsFormat.append(projectCacheKey);

    return true;
}

bool ProjectCacheBrowserModule::getDefaultValue(const JsonArray &jsonObjectInputValues,
                                                QString &defaultValue)
{
    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValues,
                                                   InputProjectCacheKeyName,
                                                   _projectCacheKey));

    QVariant value;
    RETURN_IF_FALSE(getCommonItf().getProjectCacheValue(getSequenceToken(),
                                                        _projectCacheKey,
                                                        value));

    if(value.isNull())
    {
        defaultValue = QString();
        return true;
    }

    if(!value.canConvert<QString>())
    {
        qWarning() << "The value got from project cache, thanks to the key: " << _projectCacheKey
                   <<  ", isn't a QString, can't get default value for file browser";
        defaultValue = QString();
        return false;
    }

    defaultValue = value.toString();
    return true;
}

bool ProjectCacheBrowserModule::saveChosenValue(const QString &value)
{
    return getCommonItf().setProjectCacheValue(getSequenceToken(), _projectCacheKey, value);
}
