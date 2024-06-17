// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>

#include <QByteArray>

class QCanBusFrame;


/** @brief This is the shared peak can expected message used to represent an expected message */
class APCanExpectedMsg : public QObject
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param maskIdx This is the index in the message payload where the mask to apply begins
            @param mask This is the mask to apply to the message received
            @param expectedMaskResult This is the expected value after having apply the mask on the
                                      received message
            @param parent The parent instance */
        explicit APCanExpectedMsg(quint8 maskIdx,
                                  const QByteArray &mask,
                                  const QByteArray &expectedMaskResult,
                                  QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~APCanExpectedMsg() override;

    public:
        /** @brief Get mask index */
        quint8 getMaskIdx() const { return _maskIdx; }

        /** @brief Get mask */
        const QByteArray &getMask() const { return _mask; }

        /** @brief Get expected mask result */
        const QByteArray &getExpectedMaskResult() const { return _expectedMaskResult; }

        /** @brief Check if the message given is valid
            @param messageReceived The message to test
            @param silent If false, we test the message received and display warnings if the message
                          isn't what we expect. This can be used when you only want to verify if the
                          message is really formatted as you expect
                          If true, we test if the message received is the expected one, if not, no
                          warnings are displayed in logs. This can be used when you want to test the
                          received message to find the expected one.
            @return True if no problem occurred */
        virtual bool checkIfMessageReceivedIsValid(const QCanBusFrame &messageReceived,
                                                   bool silent = false) const;

    protected:
        /** @brief Get the information from the parameters parts
            @param parametersParts The parameters parts to parse and get information from
            @param maskIdxPartIdx The index in the parameters list of the mask idx
            @param maskPartIdx The index in the paramters list of the mask
            @param expectedMaskResultPartIdx The index in the paramters list of the expected mask
                                             result
            @param maskIdx The mask id value parsed
            @param mask The mask value parsed
            @param expectedMaskResult The expected mask result parsed
            @return True if no problem occurred */
        static bool getInfoFromParameters(const QStringList &parametersParts,
                                          int maskIdxPartIdx,
                                          int maskPartIdx,
                                          int expectedMaskResultPartIdx,
                                          quint8 &maskIdx,
                                          QByteArray &mask,
                                          QByteArray &expectedMaskResult);

    private:
        quint8 _maskIdx{0};
        QByteArray _mask{};
        QByteArray _expectedMaskResult{};
};
