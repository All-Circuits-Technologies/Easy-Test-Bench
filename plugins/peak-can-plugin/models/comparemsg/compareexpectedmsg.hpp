// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/apcanexpectedmsg.hpp"
#include "numberutility/number.hpp"

class ExpectedCanFrameMask;
class QCanBusFrame;


/** @brief This is the shared peak can expected message used to represent an expected message */
class CompareExpectedMsg : public APCanExpectedMsg
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param msgIdx This is the message index used by caller class, it's needed to link the
                          verification to do with the message received
            @param maskIdx This is the index in the message payload where the mask to apply begins
            @param mask This is the mask to apply to the message received
            @param isLsb True if the endianess of the value got, thanks to the mask, is lsb first
            @param expectedMaskResult This is the expected value after having apply the mask on the
                                      received message
            @param waitUntilReceived If true, it means that we have to wait the expected answer
                                     before returning the message to be tested
            @param testNameBusinessLog This is an optionnal parameter, if it's given, it is used
                                       to display a test name in business log
            @param infTolerance This is an mandatory paramter, this is the lower limit for
                                comparison
            @param supTolerance This is an mandatory paramter, this is the upper limit for
                                comparison
            @param parent The parent instance */
        explicit CompareExpectedMsg(quint8 msgIdx,
                                    quint8 maskIdx,
                                    const QByteArray &mask,
                                    bool isLsb,
                                    const QByteArray &expectedMaskResult,
                                    bool waitUntilReceived,
                                    const QString &testNameBusinessLog = {},
                                    const Number infTolerance = {},
                                    const Number supTolerance = {},
                                    QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~CompareExpectedMsg() override;

    public:
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
                                                   bool silent = false) const override;

    public:
        /** @brief Get the message index used by caller class, it's needed to link the verification
                   to do with the message received */
        quint8 getMsgIdx() const { return _msgIdx; }

        /** @brief Get the test name to display in business log
                @note May be null */
        const Number &getCompareValue() const { return _compareValue; }

        /** @brief Get the test name to display in business log
                @note May be null */
        const Number &getInfTolerance() const { return _infTolerance; }

        /** @brief Get the test name to display in business log
                @note May be null */
        const Number &getSupTolerance() const { return _supTolerance; }

        /** @brief Get the test name to display in business log
                @note May be null */
        const QString &getTestNameBusinessLog() const { return _testNameBusinessLog; }

        /** @brief Say if we are waiting for the expected answer before returning the message to be
                       tested */
        bool hasToWaitUntilReceivedExpected() const { return _waitUntilReceived; }

    public:
        /** @brief Parse the PEAK CAN expected message thanks to the parameters given
            @param moduleParameter The stringified expected message to parse
            @param manageMultipleMsgs If true, the parent modules manages multiple messages
                                      If false, it means that the parent module expects only one
                                      message, and so the message idx is not written in
                                      @ref moduleParameter (we have to add it manually)
            @return The created @ref PCanExpectedMessage thanks to the given parameters or
                    nullptr if a problem occurred. */
        static const CompareExpectedMsg *parseExpectedMessage(const QString &moduleParameter,
                                                               bool manageMultipleMsgs = false);

        /** @brief Extract the messages which are linked to the @ref msgId from the @ref allMessages
                   list; and add them into the @ref expectMsgs list.
            @note "Extract" means that we remove the elements from @ref allMessages
            @param msgIdx The message index used to get the verification on the right message
            @param allMessages The list from where we search and extract the expected messages
            @param expectMsgs The list to fill with the extracted expected message */
        static void extractExpectedMessage(int msgIdx,
                                           QVector<const CompareExpectedMsg *> &allMessages,
                                           QVector<const CompareExpectedMsg *> &expectMsgs);

        /** @brief Generates an @ref ExpectedCanFrameMask object linked to the given expected
                       messages
                @note If @ref expectMsgs is empty or if all the expected messages
                      @ref _waitUntilReceived parameter is equal to false, the returned
                      @ref ExpectedCanFrameMask will only contain the @ref receivedMsgId
                @note The method also verifies if no mask expected results are overlapping between
                      each others. If it's the case, the method will return nullptr.
                @param receivedMsgId The id of the expected message to receive
                @param expectMsgs The expected messages to generate the @ref ExpectedCanFrameMask
                                  from. The values given here has to be linked with the
                                  @ref receivedMsgId given above and so have the same message index.
                @return The expected can frame mask or nullptr if the expected messages are
                        inconsistent */
        static const ExpectedCanFrameMask *createPeakCanMessage(
            quint32 receivedMsgId,
            const QVector<const CompareExpectedMsg *> &expectMsgs);
    private:
        /** @brief Create a @ref PCanExpectedMessage instance thanks to the @ref parametersParts
                   parameter given
            @note The method doesn't expect to have the description of business logs in the
                  @ref parametersParts parameter
            @param parametersParts The element to parse in order to create the object
            @param waitUntilReceived The value to use when the method creates the
                                     @ref PCanExpectedMessage class instance, for the wait until
                                     received parameter
            @param testNameBusinessLog The value to use when the method creates the
                                       @ref PCanExpectedMessage class instance, for the business log
                                       test name
            @return The created @ref PCanExpectedMessage thanks to the given parameters or
                    nullptr if a problem occurred. */
        static const CompareExpectedMsg *parseExpectedMessageWithoutLogsAndExt(
            const QStringList &parametersParts,
            bool waitUntilReceived = DefaultWaitUntilReceived,
            const QString &testNameBusinessLog = {});

        /** @brief Create a @ref PCanExpectedMessage instance thanks to the @ref parametersParts
                       parameter given
                @note The method doesn't expect to have the description of business logs in the
                      @ref parametersParts parameter
                @param parametersParts The element to parse in order to create the object
                @param testNameBusinessLog The value to use when the method creates the
                                           @ref PCanExpectedMessage class instance, for the business
                                           log test name
                @return The created @ref PCanExpectedMessage thanks to the given parameters or
                        nullptr if a problem occurred. */
        static const CompareExpectedMsg *parseExpectedMessageWithExtension(
            const QStringList &parametersParts,
            const QString &testNameBusinessLog = {});

        /** @brief Create a @ref PCanExpectedMessage instance thanks to the
                   @ref parametersParts parameter given
            @note The method expects to have the description of business logs in the
                  @ref parametersParts parameter
            @param parametersParts The element to parse in order to create the object
            @return The created @ref PCanWritingExpectedAnswer thanks to the given parameters or
                    nullptr if a problem occurred. */
        static const CompareExpectedMsg *parseExpectedMessageWithLogs(
            const QStringList &parametersParts);

        /** @brief parse QString list to get value needed to compare payload
            @param parametersParts The element to parse in order to create the object
            @param maskIdx index of the first value on paylod to apply the mask
            @param mask mask value
            @param isLsb True if the endianess of the value got, thanks to the mask, is lsb first
            @param infTolerance lower value for comparison
            @param supTolerance upper value for comparison
            @return true if success, false otherwise */
        static const bool parseFromParameters(const QStringList &parametersParts,
                                              quint8 &maskIdx,
                                              QByteArray &mask,
                                              bool &isLsb,
                                              Number &infTolerance,
                                              Number &supTolerance);

    private:
        /** @brief The number of expected elements in the string to parse, when there is no
                   expected business logs in it */
        static const constexpr int PartsNb = 6;

        /** @brief The number of expected elements in the string to parse, when there is the wait
                   until received info in it */
        static const constexpr int PartsNbWithExtension = 7;

        /** @brief The number of expected elements in the string to parse, when there is business
                   logs in it */
        static const constexpr int PartsNbWithLogs = 8;

        /** @brief The position in the string to parse, of the msgIdx */
        static const constexpr int MsgIdxPartIdx = 0;

        /** @brief The position in the string to parse, of the maskIdx */
        static const constexpr int MaskIdxPartIdx = 1;

        /** @brief The position in the string to parse, of the mask */
        static const constexpr int MaskPartIdx = 2;

        /** @brief The position in the string to parse, of the lsb First information */
        static const constexpr int IsLsbFirstPartIdx = 3;

        /** @brief The position in the string to parse, of the InfTolerance */
        static const constexpr int InfTolerancePartIdx = 4;

        /** @brief The position in the string to parse, of the SupTolerance */
        static const constexpr int SupTolerancePartIdx = 5;

        /** @brief The position in the string to parse, of the wait until received info, if the
                   element has been given */
        static const constexpr int WaitUntilPartIdx = 6;

        /** @brief The position in the string to parse, of the testNameLog, if the element has been
                   given */
        static const constexpr int TestNameLogPartIdx = 7;

        /** @brief The default msgIdx value to prepend when the caller module only managed one
                   message.
                   (In that case, because there is only one expected message, the string to parse
                   doesn't contain an index of command) */
        static const constexpr char *DefaultMsgIdxValue = "0";

        static const constexpr bool DefaultWaitUntilReceived = false;

    private:
        quint8 _msgIdx{0};
        bool _waitUntilReceived{DefaultWaitUntilReceived};
        quint8 _maskIdx{0};
        QByteArray _mask{};
        bool _isLsb{false};
        QString _testNameBusinessLog{};
        mutable Number _compareValue{};
        Number _infTolerance{};
        Number _supTolerance{};
};
