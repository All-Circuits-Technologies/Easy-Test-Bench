// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include <QIODevice>
#include <QRegularExpression>
#include <QString>

#include "serial-link-lib/serialpluginlibconstants.hpp"

#include "types/seriallinkreceivedreplytype.hpp"

class SerialLinkIntf;


/** @brief This abstract module let user send a message on a serial line and get the answer

    @note This module opens and close serial port on-purpose if not previously opened
    @note With specific parameters, this module can perform a write-only operation */
class ASerialLinkWriteRead : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Module constructor
            @param moduleClassName The module class name
            @param bankLinked The bank linked to the instance module
            @param parent Qt parentship */
        explicit ASerialLinkWriteRead(const QString &moduleClassName,
                                      const QSharedPointer<DefaultBankInterface> &bankLinked,
                                      QObject *parent = nullptr);

        /** @brief Module constructor
            @param moduleClassName The module class name
            @param bankLinked The bank linked to the instance module
            @param isMultiLine False if what we expect is on one line
                               True if the expected reply may be on multiple line
            @param parent Qt parentship */
        explicit ASerialLinkWriteRead(const QString &moduleClassName,
                                      const QSharedPointer<DefaultBankInterface> &bankLinked,
                                      bool isMultiLine,
                                      const QRegularExpression &expectedReply,
                                      QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ASerialLinkWriteRead() override = default;

    protected slots:
        /** @brief Execute the task
            @copydoc AbstractSerialLinkModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @brief The flags list of parameters managed by the module */
        enum ParamsManagedBySubModule
        {
            ExpectedPattern = 0x01,
            FailurePattern  = 0x02,
            EndOfLine       = 0x04,
            Trim            = 0x08,
            IsMultiLineTest = 0x10
        };

        /** @brief The flags list of inputs managed by the module */
        enum InputsManagedBySubModule
        {
            Message         = 0x01
        };

        /** @brief The flags list of outputs managed by the module */
        enum OutputsManagedBySubModule
        {
            Found         = 0x01,
            CaptureGroups = 0x02
        };

        /** @brief Precise the way we want to communicate with the serial port */
        enum OpenMode
        {
            ReadOnly    = QIODevice::ReadOnly,
            WriteOnly   = QIODevice::WriteOnly,
            ReadWrite   = QIODevice::ReadWrite
        };

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note The following elements are always added. To see the optional elements see
                  @ref addInputsFormat
            @note @ref SerialPluginLibConstants::Json::portName : string (Serial port name (ex:
                                                                          COM1)) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::initParamsFormat
            @note The following elements are always added. To see the optional elements see
                  @ref addParametersFormat
            @note @ref SerialPluginLibConstants::Json::timeout : number (Maximum time to wait for a
                                                                         line matching expected or
                                                                         failure pattern, if -1 no
                                                                         timeout)
            @note @ref SerialPluginLibConstants::Json::emitErrorIfFail : boolean (If the option is
                                                                                  active and an
                                                                                  error occurs in
                                                                                  the module, the
                                                                                  module will
                                                                                  fail) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief This is called to manage the serial link write read process in derived class
            @param jsonObjectInputValues The module inputs
            @param serialIntf This is the interface of the serial link to communicate with.
                              This has already been tested and the link is valid to use.
            @param receivedReply The method implementation has to return the received reply state
            @param capturedElements Those are the captured elements by the expected reply regex
                                    Attention: the first element is the result of whole match;
                                    therefore you have to add one to the captured element index you
                                    expect.
            @return True if a problem occurred */
        virtual bool manageWriteReadProcess(const JsonArray &jsonObjectInputValues,
                                            const QSharedPointer<SerialLinkIntf> &serialIntf,
                                            SerialLinkReceivedReplyType::Enum &receivedReply,
                                            QVector<QString> &capturedElements) = 0;

        /** @brief This is called to manage the module outputs
            @note By default, the method does nothing, the derived class may use this method to
                  add values into outputs
            @note To add the @ref SerialPluginLibConstants::Json::found and/or
                  @ref SerialPluginLibConstants::Json::capturedGroup outputs (which are not added by
                  default) you can use the method: @ref setInfoToOutput
            @param receivedReply This is the received reply status of the process
            @param capturedElements Those are the captured elements by the expected reply regex
                                    Attention: the first element is the result of whole match;
                                    therefore you have to add one to the captured element index you
                                    expect.
            @param jsonOutputValues The output array to fill
            @return True if a problem occurred */
        virtual bool manageModuleOutput(SerialLinkReceivedReplyType::Enum receivedReply,
                                        const QVector<QString> &capturedElements,
                                        JsonArray &jsonOutputValues);

        /** @brief Set the pattern expected by the command
            @param expectedPattern The expected pattern
            @return True if no problem occurred */
        virtual bool setExpectedPattern(const QString &expectedPattern);

        /** @brief Set the pattern of the fail answer, which can be given after the command sending
            @param failurePattern The failure pattern
            @return True if no problem occurred */
        bool setFailurePattern(const QString &failurePattern);


        /** @brief Manage the sending of command, and the answer reception
            @note For ReadOnly and ReadWrite mode, the old RX data are trashed before waiting for
                  new ones
            @note Most of the time, let the "ignoreStoppingInProcess" parameter to false. This
                  parameter is used to not test the stop asked by user while the command is
                  processing.
            @param serialIntf The serial link interface to communicate with
            @param messageToSend The message to send
                                 If empty, an empty message is sent
                                 If null, we considere you don't want to send message. This may
                                 return an error if no expected reply has been given (because you
                                 are doing nothing)
            @param ignoreStoppingInProcess Ignore the stop asked by user while maning the command,
                                           if one has been asked, most of the time, keep this param
                                           to "false"
            @return True if no problem occurs */
        bool manageCommandSending(const QSharedPointer<SerialLinkIntf> &serialIntf,
                                  const QString &messageToSend,
                                  QVector<QString> &capturedElements,
                                  bool ignoreStoppingInProcess = false);
        /** @brief Test if an error is returned in case the process has failed */
        bool isEmittingErrorIfFailed() const { return _emitErrorIfFail; }

        /** @brief Get the current value of the timeout in milliseconds
            @note This is the timeout when waiting for an expected message */
        int getTimeoutInMs() const { return _timeoutMs; }

        /** @brief Set the current value for the timeout in milliseconds
            @note This is the timeout when waiting for an expected message */
        void setTimeoutInMs(int timeoutInMs) { _timeoutMs = timeoutInMs; }

        /** @brief This method is helpful to get the message from input (if a message is expected
                   in input)
            @param jsonObjectInputValues The input object values
            @param message The message to get
            @return True if no problem occurred */
        bool getMessageFromInput(const JsonArray &jsonObjectInputValues, QString &message);

        /** @brief Set information to the output array depending of the @ref outputsToAdd param
            @note You may set @ref SerialPluginLibConstants::Json::found or
                  @ref SerialPluginLibConstants::Json::capturedGroup outputs
            @param outputsToAdd A flag to precise what outputs you want to add in the
                                @ref jsonOutputValues param.
                                The flag has to be a combination of the enum:
                                @ref OutputsManagedBySubModule
            @param receivedReply This is the received reply status of the process
            @param capturedElements Those are the captured elements by the expected reply regex
                                    Attention: the first element is the result of whole match;
                                    therefore you have to add one to the captured element index you
                                    expect.
            @param jsonOutputValues The output array to fill
            @return True if a problem occurred */
        bool setInfoToOutput(int outputsToAdd,
                             SerialLinkReceivedReplyType::Enum receivedReply,
                             const QVector<QString> &capturedElements,
                             JsonArray &jsonOutputValues);

    protected:
        /** @brief Set wanted optional parameters format to the given array
            @note @ref SerialPluginLibConstants::Json::expectedPattern : string (Expected success
                                                                                 pattern (regexp
                                                                                 string)
                                                                                 (ex:\"<A.*>\"))
            @note @ref SerialPluginLibConstants::Json::failurePattern : string (Possible failure
                                                                                pattern (regexp
                                                                                string)
                                                                                (ex:\".*err.*\"))
            @note @ref SerialPluginLibConstants::Json::endOfLine : string (End of line string
                                                                           (defaults to LF))
            @note @ref SerialPluginLibConstants::Json::Trim : boolean (Should received lines be
                                                                       trimmed before verifications)
            @note @ref SerialPluginLibConstants::Json::IsMultiLineTest : boolean (If true, the
                                                                                  expected regexp
                                                                                  pattern will be
                                                                                  tested with all
                                                                                  the received data.
                                                                                  If false, the
                                                                                  regexp pattern
                                                                                  will only be
                                                                                  tested with each
                                                                                  line.)
            @param paramsManaged The flag list of parameters to get (@ref ParamsManagedBySubModule)
            @param parametersFormat The wanted parameters format
            @return True if no problem occurred */
        static bool addParametersFormat(int paramsManaged, JsonArray &parametersFormat);

        /** @brief Get the parameters format managed by the abstract module
            @note @ref SerialPluginLibConstants::Json::message : string (Message to send)
            @param inputsToGet The flag list of inputs to get (@ref InputsManagedBySubModule)
            @param inputsFormat The wanted inputs format
            @return True if no problem occurred */
        static bool addInputsFormat(int inputsToGet, JsonArray &inputsFormat);

        /** @brief Get the parameters format managed by the abstract module
            @note @ref SerialPluginLibConstants::Json::found : boolean (Is expected success-line
                                                                        found)
            @note @ref SerialPluginLibConstants::Json::capturedGroup : List<string> (Contains the
                                                                                     group captured
                                                                                     from the given
                                                                                     expected
                                                                                     regexp.)
            @param outputsToGet The flag list of outputs to set (@ref OutputsManagedBySubModule)
            @param outputsFormat The wanted outputs format
            @return True if no problem occurred */
        static bool addOutputsFormat(int outputsToGet, JsonArray &outputsFormat);

    private:
        /** @brief Manage the module process
            @param jsonObjectInputValues The module input values
            @param jsonOutputValues The module output values
            @return True if no problem occurred */
        bool manageModuleProcess(const JsonArray &jsonObjectInputValues,
                                 JsonArray &jsonOutputValues);

        /** @brief Get the serial interface name from input and then try to get the serial interface
                   from the serial link lib
            @param jsonObjectInputValues The module input values
            @return nullptr if nothing has been found */
        QSharedPointer<SerialLinkIntf> getSerialIntfFromInput(
            const JsonArray &jsonObjectInputValues);

        /** @brief Manage the open of the serial interface
            @note The serial interface may already be opened by another process, in that case, the
                  param @ref serialPortOpenedByMe is set to false
            @param serialIntf The serial interface to open
            @param serialPortOpenedByMe False if the serial interface was already opened by someone
                                        else
            @return True if no problem occurred */
        bool manageIntfOpen(const QSharedPointer<SerialLinkIntf> &serialIntf,
                            bool &serialPortOpenedByMe);

        /** @brief Send the command to the serial link interface
            @attention Here, we expect @ref messageToSend to be not null (we don't test it, we
                       expect is done before)
            @param serialIntf The serial link interface to use to send message
            @param messageToSend The message to send
            @return True if no problem occurred */
        bool sendCommand(const QSharedPointer<SerialLinkIntf> &serialIntf,
                         const QString &messageToSend);

        /** @brief Manage the read and/or write of the command
            @param serialIntf The serial link interface to use to send message and listen messages
            @param messageToSend The message to send, if null no message are sent
            @param ignoreStoppingInProcess Ignore the stop asked by user while maning the command,
                                           if one has been asked, most of the time, keep this param
                                           to "false"
            @param receivedReply The method implementation has to return the received reply state
            @param capturedElements Those are the captured elements by the expected reply regex
                                    Attention: the first element is the result of whole match;
                                    therefore you have to add one to the captured element index you
                                    expect.
            @return True if no problem occurred */
        bool manageReadAndOrWriteCmd(const QSharedPointer<SerialLinkIntf> &serialIntf,
                                     const QString &messageToSend,
                                     bool ignoreStoppingInProcess,
                                     SerialLinkReceivedReplyType::Enum &receivedReply,
                                     QVector<QString> &capturedElements);

    private:
        static const constexpr char *InputMessageDesc = "Message to send";

        static const constexpr char *OutputFoundDesc = "Is expected success-line found";

        static const constexpr char *OutputCaptureGroupsDesc = "Contains the group captured from "
                                                               "the given expected regexp.";

        static const constexpr char *TimeoutParamDesc = "Maximum time to wait for a line matching "
                                                        "expected or failure pattern, if -1 no "
                                                        "timeout";

        static const constexpr char *EmitErrorIfFailParamDesc = "If the option is active and an "
                                                                "error occurs in the module, the "
                                                                "module will fail";

        static const constexpr char *FailurePatternParamDesc = "Possible failure pattern (regexp "
                                                               "string) (ex:\".*err.*\")";

        static const constexpr char *ExpectedPatternParamDesc = "Expected success pattern (regexp "
                                                                "string) (ex:\"<A.*>\")";

        /** @brief This is the default timeout in milliseconds */
        static const constexpr int DefaultTimeoutInMs = -1;

        /** @brief This is the default emit error if fail state */
        static const constexpr bool DefaultEmitErrorIfFail = false;

        /** @brief This is the default expected regex to parse the reply */
        static const QRegularExpression DefaultExpectedReply;

        /** @brief This is the default possible failure regex to parse the reply */
        static const QRegularExpression DefaultPossibleFailure;

    private:
        QRegularExpression _expectedReply;
        QRegularExpression _possibleFailure{DefaultPossibleFailure};
        int _timeoutMs{DefaultTimeoutInMs};
        QString _eol{SerialPluginLibConstants::Defaults::EndOfLine};
        bool _trim{SerialPluginLibConstants::Defaults::Trim};
        bool _emitErrorIfFail{DefaultEmitErrorIfFail};
        bool _isMultiLineTest;

        OpenMode _openMode{ReadWrite};
};
