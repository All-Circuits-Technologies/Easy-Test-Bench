// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class AutoAnsExpectedMessage;
class CanMsgSeqParams;
class QCanBusFrame;


/** @brief This is the auto answer sequence info
    @note The method @ref parsePCanSequenceInfo makes the class data consistent */
class AutoAnsSequenceInfo : public QObject
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param expectedMessages The expected messages to use
            @param parent The parent instance class */
        explicit AutoAnsSequenceInfo(const QVector<const AutoAnsExpectedMessage*> &expectedMessages,
                                     QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~AutoAnsSequenceInfo();

    public:
        /** @brief Get the list of the commands to send */
        const QVector<const AutoAnsExpectedMessage*> &getExpectedMessages() const
        { return _expectedMessages; }

        /** @brief Get the list of the expected messages which match the received messsage
            @note If empty it means that the message received wasn't expected
            @param messageReceived The message received to test
            @return The list of the matched elements */
        QVector<const AutoAnsExpectedMessage*> getMatchedElements(
            const QCanBusFrame &messageReceived) const;

    public:
        /** @brief Create a @ref AutoAnsSequenceInfo thanks to the message sequence parameters
            @param msgSeqParams The module sequence parameters
            @return The created @ref AutoAnsSequenceInfo or nullptr if a problem occurred */
        static const AutoAnsSequenceInfo *parsePCanSequenceInfo(
            const CanMsgSeqParams &msgSeqParams);

    private:
        QVector<const AutoAnsExpectedMessage*> _expectedMessages;
};
