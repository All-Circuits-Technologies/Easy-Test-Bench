// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "testifcachevalueexistmodule.hpp"

#include "bankcoreshared/usebyplugin/icommoninterface.hpp"
#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/parsers/bankjsonparser.hpp"
#include "testbedcore/testbedglobal.hpp"


TestIfCacheValueExistModule::TestIfCacheValueExistModule(QObject *parent) :
    AProjectCacheModule(staticMetaObject.className(), parent)
{
}

TestIfCacheValueExistModule::~TestIfCacheValueExistModule()
{
}

bool TestIfCacheValueExistModule::initOutputsFormat(JsonArray &outputsFormat) const
{
    RETURN_IF_FALSE(AProjectCacheModule::initOutputsFormat(outputsFormat));

    JsonObject valueExist;
    RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(OutputCacheValueExistName,
                                                       valueExist,
                                                       OutputCacheValueExistDesc));

    outputsFormat.append(valueExist);

    return true;
}

bool TestIfCacheValueExistModule::actOnCache(const JsonArray &jsonObjectInputValue,
                                             const QString &projectCacheKey,
                                             JsonArray &output)
{
    Q_UNUSED(jsonObjectInputValue)

    QVariant value;
    RETURN_IF_FALSE(getCommonItf().getProjectCacheValue(getSequenceToken(),
                                                        projectCacheKey,
                                                        value));

    return BankJsonParser::setBoolValue(OutputCacheValueExistName,
                                        !value.isNull(),
                                        output);
}
