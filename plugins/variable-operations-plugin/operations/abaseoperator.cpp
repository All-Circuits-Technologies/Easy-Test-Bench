// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "abaseoperator.hpp"

#include "abstractpluginelements/defaultbankglobalmanager.hpp"

#include "testbedcore/parsers/bankjsonformatter.hpp"
#include "testbedcore/testbedglobal.hpp"

#include <QTimer>


ABaseOperator::ABaseOperator(const QString &moduleClassName,
                             TypeManaged::Enum outputType,
                             QObject *parent) :
    DefaultSequenceInstanceModule(moduleClassName,
                                  DefaultBankGlobalManager::Instance(),
                                  parent),
    _outputType(outputType)
{
}

bool ABaseOperator::initOutputsFormat(JsonArray &outputsFormat) const
{
    return addInOutFormat(outputPinName, outputPinDesc, _outputType, outputsFormat);
}

bool ABaseOperator::addInOutFormat(const QString &inOutName,
                                   const QString &inOutDesc,
                                   TypeManaged::Enum inOutType,
                                   JsonArray &inOutFormat)
{
    JsonObject inputElement;

    switch(inOutType)
    {
        case TypeManaged::Boolean:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutBool(inOutName,
                                                               inputElement,
                                                               inOutDesc));
            break;
        case TypeManaged::Number:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumber(inOutName,
                                                                 inputElement,
                                                                 inOutDesc));
            break;
        case TypeManaged::String:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutString(inOutName,
                                                                 inputElement,
                                                                 inOutDesc));
            break;
        case TypeManaged::BooleanList:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutBoolArray(inOutName,
                                                                    inputElement,
                                                                    inOutDesc));
            break;
        case TypeManaged::NumberList:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutNumberArray(inOutName,
                                                                      inputElement,
                                                                      inOutDesc));
            break;
        case TypeManaged::StringList:
            RETURN_IF_FALSE(BankJsonFormatter::formatInOutStringArray(inOutName,
                                                                      inputElement,
                                                                      inOutDesc));
            break;
        case TypeManaged::Unknown:
            qWarning() << "The managed type of the input or output element is unknown";
            return false;
    }

    inOutFormat.append(inputElement);

    return true;
}

void ABaseOperator::processModule(const JsonArray &jsonObjectInputValues)
{
    JsonArray outputValues;

    bool success = operate(jsonObjectInputValues, outputValues);

    emit moduleFinished(success, outputValues);
}
