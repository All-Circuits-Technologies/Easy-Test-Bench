// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/apcanexpectedmsg.hpp"

class AutoAnsSequActionInfo;


/** @brief This is the auto answer expected message representation
    @note The method @ref parseExpectedMessage makes the class data consistent */
class AutoAnsExpectedMessage : public APCanExpectedMsg
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param msgId The CAN expected message id
            @param msgIdx This is the message index used by caller class, it's needed to link the
                          verification to do with the message received
            @param maskIdx This is the index in the message payload where the mask to apply begins
            @param mask This is the mask to apply to the message received
            @param expectedMaskResult This is the expected value after having apply the mask on the
                                      received message
            @param seqActions Contains the commands to send after having received the expected
                              message
            @param parent The parent instance */
        explicit AutoAnsExpectedMessage(quint32 msgId,
                                        quint8 maskIdx,
                                        const QByteArray &mask,
                                        const QByteArray &expectedMaskResult,
                                        QVector<const AutoAnsSequActionInfo *> seqActions,
                                        QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~AutoAnsExpectedMessage() override;

    public:
        /** @brief Get the id of the expected message */
        quint32 getMsgId() const { return _msgId; }

        /** @brief Get the commands to send after having received the expected message */
        const QVector<const AutoAnsSequActionInfo *> &getSeqActions() const { return _seqActions; }

        /** @copydoc APCanExpectedMsg::checkIfMessageReceivedIsValid */
        virtual bool checkIfMessageReceivedIsValid(const QCanBusFrame &messageReceived,
                                                   bool silent = false) const override;

    public:
        /** @brief Create a @ref AutoAnsExpectedMessage thanks to the module parameter set in the
                   sequence file
            @param moduleParameter This is the module parameter to parse in order to create the
                                   expected message
            @param seqActions Contains the commands to send after having received the expected
                              message
            @return The created @ref moduleParameter thanks to the given parameters or nullptr if
                    a problem occurred */
        static const AutoAnsExpectedMessage *parseExpectedMessage(
            const QString &moduleParameter,
            const QVector<const AutoAnsSequActionInfo *> &seqActions);

    private:
        /** @brief The number of parts in the module parameter given */
        static const constexpr int PartsNb = 4;

        /** @brief The position in the string to parse, of the msgId */
        static const constexpr int MsgIdPartIdx = 0;

        /** @brief The position in the string to parse, of the maskIdx */
        static const constexpr int MaskIdxPartIdx = 1;

        /** @brief The position in the string to parse, of the mask */
        static const constexpr int MaskPartIdx = 2;

        /** @brief The position in the string to parse, of the expectedMaskResult */
        static const constexpr int ExpectedMaskResultPartIdx = 3;

    private:
        quint32 _msgId;
        QVector<const AutoAnsSequActionInfo *> _seqActions;
};
