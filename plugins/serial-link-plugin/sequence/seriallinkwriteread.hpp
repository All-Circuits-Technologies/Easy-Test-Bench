// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "serial-link-lib/aseriallinkwriteread.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief Implementation of the @ref ASerialLinkWriteRead class */
class SerialLinkWriteRead : public ASerialLinkWriteRead
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(SerialLinkWriteRead)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit SerialLinkWriteRead(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~SerialLinkWriteRead() override;

    protected:
        /** @copydoc ASerialLinkWriteRead::initInputsFormat
            @note Add the @ref ASerialLinkWriteRead @ref InputsManagedBySubModule::Message input */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc ASerialLinkWriteRead::initParamsFormat
            @note Add the @ref ASerialLinkWriteRead @ref ParamsManagedBySubModule::ExpectedPattern
                  param
            @note Add the @ref ASerialLinkWriteRead @ref ParamsManagedBySubModule::FailurePattern
                  param
            @note Add the @ref ASerialLinkWriteRead @ref ParamsManagedBySubModule::EndOfLine param
            @note Add the @ref ASerialLinkWriteRead @ref ParamsManagedBySubModule::Trim param
            @note Add the @ref ASerialLinkWriteRead @ref ParamsManagedBySubModule::IsMultiLineTest
                  param */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc ASerialLinkWriteRead::initOutputsFormat
            @note Add the @ref ASerialLinkWriteRead @ref OutputsManagedBySubModule::Found output
            @note Add the @ref ASerialLinkWriteRead @ref OutputsManagedBySubModule::CaptureGroups
                  output */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @copydoc ASerialLinkWriteRead::manageWriteReadProcess */
        virtual bool manageWriteReadProcess(
            const JsonArray &jsonObjectInputValues,
            const QSharedPointer<SerialLinkIntf> &serialIntf,
            SerialLinkReceivedReplyType::Enum &receivedReply,
            QVector<QString> &capturedElements) override;

        /** @copydoc ASerialLinkWriteRead::manageModuleOutput */
        virtual bool manageModuleOutput(
            SerialLinkReceivedReplyType::Enum receivedReply,
            const QVector<QString> &capturedElements,
            JsonArray &jsonOutputValues) override;
};
