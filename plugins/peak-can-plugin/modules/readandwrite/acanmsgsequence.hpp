// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include <QCanBusFrame>

class CanMsgSeqParams;
class PCanSequenceInfo;


/** @brief This is the base for modules which manages the sending or receiving CAN messages */
class ACanMsgSequence : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    protected:
        /** @brief Class constructor
            @param moduleClassName The class name of the module
            @param manageMultipleMsgs If true, it means that we manage multiple message sending or
                                      receiving in the module process
            @param exportReceivedMsgs If true, that means we export the received message(s) to the
                                      module output
            @param parent The parent instance class */
        explicit ACanMsgSequence(const QString &moduleClassName,
                                 bool manageMultipleMsgs,
                                 bool exportReceivedMsgs,
                                 QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~ACanMsgSequence() override;

    protected:
        /** @brief Say if the class is managing multiple message sending or receiving in the module
                   process
            @return True if the module manages multiple messages */
        bool isManagingMultipleMsgs() const { return _manageMultipleMsgs; }

        /** @brief Say if the class is exporting the received messages (if there is any)
            @return True if the module manages messages exporting */
        bool isExportingReceivedMsgs() const { return _exportReceivedMsgs; }

    protected:
        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note @ref InputPeakCanIntfName : string (The input contains the name of the peak CAN
                                                      interface) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::initParamsFormat
            @note This method contains the default parameters format, to see the one applied to
                  multiple or single message, see the doc of @ref initMultipleMsgsParamsFormat or
                  @ref initSingleMsgParamsFormat methods
            @note @ref TestGroupNameParamName : string (The name of the test groups, which will be
                                                        displayed in logs files.)
            @note @ref ExpectingMsgTimeoutParamName : number (The timeout linked to the expected
                                                              msg(s). -1 if we wait indefinitely.)
            @note @ref EmitErrorIfFailParamName : boolean (If the option is active and a message
                                                           verification failed in the module, it
                                                           will fail) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @brief Adds the format of the parameters used when working with multiple messages
                   sending or receiving
            @note @ref ExpectedMsgsIdsParamName : List<number> (The ID list of the expected
                                                                messages, if one message id is
                                                                equals to -1, it means that no
                                                                message is expected.)
            @note @ref ExpectedMsgsVerifsParamName : List<string> (This contains the elements to
                                                                   verify in the received
                                                                   messages. The info are stored
                                                                   in a string with the following
                                                                   format:
                                                                   \"MSG_IDX/MASK_IDX/MASK/
                                                                   MASK_RESULT\", "
                                                                   if the payload of the message
                                                                   matches the mask and its
                                                                   result, everything is right.
                                                                   The message index matches the
                                                                   expected messages list index
                                                                   and it says that the
                                                                   verification has to be applied
                                                                   on the message received for
                                                                   with the given index.
                                                                   It's possible to add logs to
                                                                   the test, to do so, you have
                                                                   to append the following
                                                                   information with the
                                                                   format :
                                                                   \"MSG_IDX/MASK_IDX/MASK/
                                                                   MASK_RESULT/TEST_NAME/"
                                                                   LOG_IF_SUCCESS/LOG_IF_FAIL\") */
        virtual bool initMultipleMsgsParamsFormat(JsonArray &paramsFormat) const;

        /** @brief Adds the format of the parameters used when working with single message
                   sending or receiving
            @note @ref ExpectedMsgIdParamName : number (The ID of the expected message, if equals
                                                        to -1, no answer is expected.)
            @note @ref ExpectedMsgVerifsParamName : string (This contains the elements to verify in
                                                            the message received. The info are
                                                            stored in a string with the following
                                                            format: \"MASK_IDX/MASK/MASK_RESULT\",
                                                            if the payload of the message matches
                                                            the mask and its result, everything is
                                                            right. It's possible to add logs to the
                                                            test, to do so, you have to append the
                                                            following information with the format :
                                                            \"MASK_IDX/MASK/MASK_RESULT/TEST_NAME/
                                                            LOG_IF_SUCCESS/LOG_IF_FAIL\") */
        virtual bool initSingleMsgParamsFormat(JsonArray &paramsFormat) const;

        /** @copydoc DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref OutputReceivedAnswersName : List<string> (Contains the list of received
                                                                 answers) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @brief Get the description of the messages ids parameter
            @return The parameter description */
        virtual QString getExpectedMsgsIdsParamDesc() const;

        /**  @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief Parse and get the needed information for @ref _sequenceInfo from the module
                   parameters
            @note All the references (and non constant) parameters are filled by the method to be
                  used by the caller
            @param parameters The object is filled with the parameters parsed
            @param canMsgSeqParams The message sequence parameters to parse
            @param errors The occurred errors in the parsing process
            @return True if no problem occurred */
        virtual bool parseInfoFromParameters(const JsonArray &parameters,
                                             CanMsgSeqParams &canMsgSeqParams,
                                             JsonArray &errors);

        /** @brief Parse and get the needed information for @ref _sequenceInfo from the module
                   parameters, in the case where we are managing multiple messages
            @note All the references (and non constant) parameters are filled by the method to be
                  used by the caller
            @param parameters The object is filled with the parameters parsed
            @param canMsgSeqParams The message sequence parameters to parse
            @param errors The occurred errors in the parsing process
            @return True if no problem occurred */
        virtual bool parseMultipleMsgsInfoFromParameters(const JsonArray &parameters,
                                                         CanMsgSeqParams &canMsgSeqParams,
                                                         JsonArray &errors);

        /** @brief Parse and get the needed information for @ref _sequenceInfo from the module
                   parameters, in the case where we are managing single messages
            @note All the references (and non constant) parameters are filled by the method to be
                  used by the caller
            @param parameters The object is filled with the parameters parsed
            @param canMsgSeqParams The message sequence parameters to parse to create the class
            @param errors The occurred errors in the parsing process
            @return True if no problem occurred */
        virtual bool parseSingleMsgInfoFromParameters(const JsonArray &parameters,
                                                      CanMsgSeqParams &canMsgSeqParams,
                                                      JsonArray &errors);

        /** @brief Manage the filling of the output with the received frames, if
                   @ref _exportReceivedMsgs is equals to true
            @param receivedFrames The frames received to add in the output, if necessary
            @param output The module output to fill */
        virtual bool manageOutputFilling(const QVector<QCanBusFrame> &receivedFrames,
                                         JsonArray &output);

    protected slots:
        /** @brief Execute the task
            @copydoc DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Do the actions expected by the module
            @return True if no problem occurred  */
        bool doCanActions(const JsonArray &jsonObjectInputValues, JsonArray &output);

    private:
        static const constexpr char *EmitErrorIfFailParamName = "exitIfFail";
        static const constexpr char *EmitErrorIfFailParamDesc = "If the option is active and a "
                                                                "message verification failed in "
                                                                "the module, it will fail";
        static const constexpr bool EmitErrorIfFailDefaultValue = false;

        static const constexpr char *ExpectedMsgIdParamName = "expectedMsgId";
        static const constexpr char *ExpectedMsgIdParamDesc = "The ID of the expected message, if "
                                                              "equals to -1, no answer is "
                                                              "expected.";
        static const constexpr qint32 ExpectedMsgIdDefaultValue = -1;

        static const constexpr char *ExpectedMsgsIdsParamName = "expectedMsgsIds";
        static const constexpr char *ExpectedMsgsIdsParamDesc = "The ID list of the expected "
                                                                "messages, if one message id is "
                                                                "equals to -1, it means that no "
                                                                "message is expected.";

        static const constexpr char *ExpectingMsgTimeoutParamName = "expectingMsgTimeoutInMs";
        static const constexpr char *ExpectingMsgTimeoutParamDesc = "The timeout linked to the "
                                                                    "expected msg(s). -1 if we "
                                                                    "wait indefinitely.";
        static const constexpr qint32 ExpectingMsgTimeoutDefaultValue = -1;

        static const constexpr char *TestGroupNameParamName = "testGroupName";
        static const constexpr char *TestGroupNameParamDesc = "The name of the test groups, which "
                                                              "will be displayed in logs files.";

        static const constexpr char *ExpectedMsgVerifsParamName = "expectedMsgVerifs";
        static const constexpr char *ExpectedMsgVerifsParamDesc = "This contains the elements to "
                                                                  "verify in the message received. "
                                                                  "The info are stored in a string "
                                                                  "with the following format: "
                                                                  "\"MASK_IDX/MASK/MASK_RESULT\", "
                                                                  "if the payload of the message "
                                                                  "matches the mask and its "
                                                                  "result, everything is right. "
                                                                  "It's possible to add a boolean "
                                                                  "to wait the right value before "
                                                                  "returning. To do so, you have "
                                                                  "to append the following "
                                                                  "information with the format: "
                                                                  "\"MASK_IDX/MASK/MASK_RESULT/"
                                                                  "WAIT_UNTIL_RECEIVED\". "
                                                                  "It's possible to add logs to "
                                                                  "the test, to do so, you have "
                                                                  "to append the following "
                                                                  "information with the format: "
                                                                  "\"MASK_IDX/MASK/MASK_RESULT/"
                                                                  "WAIT_UNTIL_RECEIVED/TEST_NAME/"
                                                                  "LOG_IF_SUCCESS/LOG_IF_FAIL\"";

        static const constexpr char *ExpectedMsgsVerifsParamName = "expectedMsgsVerifs";
        static const constexpr char *ExpectedMsgsVerifsParamDesc = "This contains the elements to "
                                                                   "verify in the received "
                                                                   "messages. The info are stored "
                                                                   "in a string with the following "
                                                                   "format: "
                                                                   "\"MSG_IDX/MASK_IDX/MASK/"
                                                                   "MASK_RESULT\", "
                                                                   "if the payload of the message "
                                                                   "matches the mask and its "
                                                                   "result, everything is right. "
                                                                   "The message index matches the "
                                                                   "expected messages list index "
                                                                   "and it says that the "
                                                                   "verification has to be applied "
                                                                   "on the message received for "
                                                                   "with the given index. "
                                                                   "It's possible to add a boolean "
                                                                   "to wait the right value before "
                                                                   "returning. To do so, you have "
                                                                   "to append the following "
                                                                   "information with the format: "
                                                                   "\"MSG_IDX/MASK_IDX/MASK/"
                                                                   "MASK_RESULT/"
                                                                   "WAIT_UNTIL_RECEIVED\""
                                                                   "It's possible to add logs to "
                                                                   "the test, to do so, you have "
                                                                   "to append the following "
                                                                   "information with the "
                                                                   "format: "
                                                                   "\"MSG_IDX/MASK_IDX/MASK/"
                                                                   "MASK_RESULT/"
                                                                   "WAIT_UNTIL_RECEIVED/TEST_NAME/"
                                                                   "LOG_IF_SUCCESS/LOG_IF_FAIL\"";

        static const constexpr char *OutputReceivedAnswersName = "outputReceivedAnswers";
        static const constexpr char *OutputReceivedAnswersDesc = "Contains the list of received "
                                                                 "answers";

    private:
        bool _manageMultipleMsgs{false};
        bool _exportReceivedMsgs{false};
        bool _exitIfFails{EmitErrorIfFailDefaultValue};

        const PCanSequenceInfo *_sequenceInfo{nullptr};
};
