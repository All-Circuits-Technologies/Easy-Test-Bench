// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "getstringcachevaluemodule.hpp"

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


GetStringCacheValueModule::GetStringCacheValueModule(QObject *parent) :
    AProjectCacheModule(staticMetaObject.className(), parent)
{
}

GetStringCacheValueModule::~GetStringCacheValueModule()
{
}

bool GetStringCacheValueModule::initOutputsFormat(JsonArray &outputsFormat) const
{
    RETURN_IF_FALSE(AProjectCacheModule::initOutputsFormat(outputsFormat));

    JsonObject cacheValue;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(OutputCacheValueName,
                                                         cacheValue,
                                                         OutputCacheValueDesc));

    outputsFormat.append(cacheValue);

    return true;
}

bool GetStringCacheValueModule::actOnCache(const JsonArray &jsonObjectInputValue,
                                           const QString &projectCacheKey,
                                           JsonArray &output)
{
    Q_UNUSED(jsonObjectInputValue)

    QVariant value;
    RETURN_IF_FALSE(getCommonItf().getProjectCacheValue(getSequenceToken(),
                                                        projectCacheKey,
                                                        value));

    QString strValue;
    if(!value.isNull())
    {
        if(!value.canConvert<QString>())
        {
            qWarning() << "The value got from project cache, thanks to the key: " << projectCacheKey
                       <<  ", isn't a QString, can't return it";
            return false;
        }

        strValue = value.toString();
    }

    return BankJsonParser::setStringValue(OutputCacheValueName, strValue, output);
}
