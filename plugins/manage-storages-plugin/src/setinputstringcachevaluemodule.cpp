// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "setinputstringcachevaluemodule.hpp"

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


SetInputStringCacheValueModule::SetInputStringCacheValueModule(QObject *parent) :
    AProjectCacheModule(staticMetaObject.className(), parent)
{
}

SetInputStringCacheValueModule::~SetInputStringCacheValueModule()
{
}

bool SetInputStringCacheValueModule::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(AProjectCacheModule::initInputsFormat(inputsFormat));

    JsonObject valueToSet;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(InputValueToSetName,
                                                         valueToSet,
                                                         InputValueToSetName));

    inputsFormat.append(valueToSet);

    return true;
}

bool SetInputStringCacheValueModule::actOnCache(const JsonArray &jsonObjectInputValue,
                                                const QString &projectCacheKey,
                                                JsonArray &output)
{
    Q_UNUSED(output)

    QString valueToSet;
    RETURN_IF_FALSE(BankJsonParser::getStringValue(jsonObjectInputValue,
                                                   InputValueToSetName,
                                                   valueToSet));

    return getCommonItf().setProjectCacheValue(getSequenceToken(), projectCacheKey, valueToSet);
}
