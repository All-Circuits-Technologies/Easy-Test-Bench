// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "seriallinkwriteread.hpp"

#include "managers/seriallinkbankglobalmanager.hpp"


SerialLinkWriteRead::SerialLinkWriteRead(QObject *parent) :
    ASerialLinkWriteRead(staticMetaObject.className(),
                         SerialLinkBankGlobalManager::Instance(),
                         parent)
{
}

SerialLinkWriteRead::~SerialLinkWriteRead()
{
}

bool SerialLinkWriteRead::initInputsFormat(JsonArray &inputsFormat) const
{
    RETURN_IF_FALSE(ASerialLinkWriteRead::initInputsFormat(inputsFormat));

    return addInputsFormat(InputsManagedBySubModule::Message, inputsFormat);
}

bool SerialLinkWriteRead::initParamsFormat(JsonArray &paramsFormat) const
{
    RETURN_IF_FALSE(ASerialLinkWriteRead::initParamsFormat(paramsFormat));

    return addParametersFormat(ParamsManagedBySubModule::ExpectedPattern  |
                               ParamsManagedBySubModule::FailurePattern   |
                               ParamsManagedBySubModule::EndOfLine        |
                               ParamsManagedBySubModule::Trim             |
                               ParamsManagedBySubModule::IsMultiLineTest,
                               paramsFormat);
}

bool SerialLinkWriteRead::initOutputsFormat(JsonArray &outputsFormat) const
{
    RETURN_IF_FALSE(ASerialLinkWriteRead::initOutputsFormat(outputsFormat));

    return addOutputsFormat(OutputsManagedBySubModule::Found |
                            OutputsManagedBySubModule::CaptureGroups,
                            outputsFormat);
}

bool SerialLinkWriteRead::manageWriteReadProcess(
    const JsonArray &jsonObjectInputValues,
    const QSharedPointer<SerialLinkIntf> &serialIntf,
    SerialLinkReceivedReplyType::Enum &/*receivedReply*/,
    QVector<QString> &capturedElements)
{
    QString message;
    RETURN_IF_FALSE(getMessageFromInput(jsonObjectInputValues, message));

    return manageCommandSending(serialIntf, message, capturedElements);
}

bool SerialLinkWriteRead::manageModuleOutput(SerialLinkReceivedReplyType::Enum receivedReply,
                                             const QVector<QString> &capturedElements,
                                             JsonArray &jsonOutputValues)
{
    return setInfoToOutput(OutputsManagedBySubModule::Found |
                           OutputsManagedBySubModule::CaptureGroups,
                           receivedReply,
                           capturedElements,
                           jsonOutputValues);
}
