// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

class AutoAnsMsgToSend;


/** @brief This represents the automatic answers to send when the messages received match a
           particular regular expression */
class AutoAnsExpectedSerialMsg : public QObject
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param expectedMessage The regexp to test the messages received.
                                   We considere that the given regular expression will be now
                                   managed by this class; therefore, this class will manage it's
                                   destroying
            @param msgsToSend The messages to send
            @param parent The parent instance class */
        explicit AutoAnsExpectedSerialMsg(QRegularExpression *expectedMessage,
                                          const QVector<AutoAnsMsgToSend *> msgsToSend,
                                          QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AutoAnsExpectedSerialMsg() override;

    public:
        /** @brief Get the expected message pattern */
        const QRegularExpression *getExpectedMsg() const { return _expectedMessage; }

        /** @brief Get the messages to send */
        const QVector<AutoAnsMsgToSend*> &getMsgsToSend() const { return _msgsToSend; }

        /** @brief Access the multi line start index from which we test the expected regular
                   expression on the global line */
        int &accessMultiLineStartIndex() { return _multiLineStartIndex; }

    private:
        QRegularExpression *_expectedMessage{nullptr};
        QVector<AutoAnsMsgToSend*> _msgsToSend;
        int _multiLineStartIndex{0};
};
