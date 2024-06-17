// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "readstrsettingmodule.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


ReadStrSettingModule::ReadStrSettingModule(QObject *parent) :
    AReadSettingModule(staticMetaObject.className(), parent)
{
}

bool ReadStrSettingModule::initOutputsFormat(JsonArray &outputsFormat) const
{
    RETURN_IF_FALSE(AReadSettingModule::initOutputsFormat(outputsFormat));

    JsonObject settingValue;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(OutputSettingValueName,
                                                         settingValue,
                                                         OutputSettingValueDesc));

    outputsFormat.append(settingValue);

    return true;
}

bool ReadStrSettingModule::formatOutputWithReadValue(const QVariant &value, JsonArray &output)
{
    if(!value.canConvert<QString>())
    {
        qWarning() << "The value got from setting: " << getSettingKey() << ", can't be converted "
                   << "to QString";
        return false;
    }
    
    return BankJsonParser::setStringValue(OutputSettingValueName, value.toString(), output);
}
