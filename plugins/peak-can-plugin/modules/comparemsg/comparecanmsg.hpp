// SPDX-FileCopyrightText: 2023 Julien Lemasson <julien.lemasson@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"

#include <QCanBusFrame>

class CanMsgSeqParams;
class CompareCanSequenceInfo;
class AutoAnsThreadHandler;


/** @brief This module sends a CAN message and extract a value from the expected answer received.
           The value extracted is tested and logs are emitted. */
class CompareCanMsg : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(CompareCanMsg)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit CompareCanMsg(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CompareCanMsg() override;

    protected:

        /** @copydoc DefaultSequenceInstanceModule::initInputsFormat
            @note @ref PeakCanLibConstants::ModulesEntries::Inputs::InputPeakCanIntfName : string
                                            (The input contains the name of the peak CAN interface)
                                            */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::initParamsFormat
            @note This method contains the default parameters format, to see the one applied to
                      single message, see the doc of @ref initSingleMsgParamsFormat methods
            @note @ref TestGroupNameParamName : string (The name of the test groups, which will be
                                                        displayed in logs files.)
            @note @ref ExpectingMsgTimeoutParamName : number (The timeout linked to the expected
                                                              msg(s). -1 if we wait indefinitely.)
            @note @ref EmitErrorIfFailParamName : boolean (If the option is active and a message
                                                           verification failed in the module, it
                                                           will fail)
            @note @ref BitrateSwitchParamName : bool (Activate, or not, the bitrate switch. This is
                                                      only relevant when using CAN FD.)
            @note @ref ExtendedFrameFormatParamName : bool (Activate, or not, the extended frame
                                                            format for the message sent.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

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
                                                            \"MASK_IDX/MASK/INF_TOLERANCE/
                                                            SUP_TOLERANCE/TEST_NAME/LOG_IF_SUCCESS/
                                                            LOG_IF_FAIL\") */
        virtual bool initSingleMsgParamsFormat(JsonArray &paramsFormat) const ;

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

    protected slots:
        /** @brief Execute the task
            @copydoc DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Do the actions expected by the module
            @return True if no problem occurred  */
        bool doCompareCanActions(const JsonArray &jsonObjectInputValues, JsonArray &output);

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

        static const constexpr char *BitrateSwitchParamName = "bitrateSwitch";
        static const constexpr char *BitrateSwitchParamDesc = "Activate, or not, the bitrate "
                                                              "switch. This is only relevant when "
                                                              "using CAN FD.";

        static const constexpr char *ExtendedFrameFormatParamName = "extendedFrameFormat";
        static const constexpr char *ExtendedFrameFormatParamDesc = "Activate, or not, the "
                                                                    "extended frame format for the "
                                                                    "message sent.";

        static const constexpr char *TestGroupNameParamName = "testGroupName";
        static const constexpr char *TestGroupNameParamDesc = "The name of the test groups, which "
                                                              "will be displayed in logs files.";

        static const constexpr char *ExpectingMsgTimeoutParamName = "expectingMsgTimeoutInMs";
        static const constexpr char *ExpectingMsgTimeoutParamDesc = "The timeout linked to the "
                                                                    "expected msg(s). -1 if we "
                                                                    "wait indefinitely.";
        static const constexpr qint32 ExpectingMsgTimeoutDefaultValue = -1;

        static const constexpr char *CanMsgToSendParamName = "canMsgToSend";
        static const constexpr char *CanMsgToSendParamDesc = "The CAN MSG to send. The message is "
                                                             "stored in a string with the "
                                                             "following format: "
                                                             "\"CAN_ID/PAYLOAD_LENGTH/PAYLOAD\"";

        static const constexpr char *ExpectedMsgVerifsParamName = "expectedMsgVerifs";
        static const constexpr char *ExpectedMsgVerifsParamDesc = "This contains the elements to "
                                                                  "verify in the message received. "
                                                                  "The info are stored in a string "
                                                                  "with the following format: "
                                                                  "\"MASK_IDX/MASK/IS_LSB/"
                                                                  "INF_TOLERANCE/SUP_TOLERANCE\", "
                                                                  "if the payload of the message "
                                                                  "with the mask is between "
                                                                  "INF_TOLERANCE and "
                                                                  "SUP_TOLERANCE, everything "
                                                                  "is right. It's possible to add "
                                                                  "a boolean to wait the right "
                                                                  "value before returning. To do "
                                                                  "so, you have to append the "
                                                                  "following information with the "
                                                                  "format: \"MASK_IDX/MASK/IS_LSB/"
                                                                  "INF_TOLERANCE/SUP_TOLERANCE/"
                                                                  "WAIT_UNTIL_RECEIVED\". It's "
                                                                  "possible to add logs to the "
                                                                  "test, to do so, you have to "
                                                                  "append the following "
                                                                  "information with the format: "
                                                                  "\"MASK_IDX/MASK/IS_LSB/"
                                                                  "INF_TOLERANCE/SUP_TOLERANCE/"
                                                                  "WAIT_UNTIL_RECEIVED/"
                                                                  "TEST_NAME";
    private:

        bool _manageMultipleMsgs{false};
        bool _exportReceivedMsgs{false};
        bool _exitIfFails{false};

        CanMsgSeqParams *_msgSeqParams{nullptr};
        const CompareCanSequenceInfo *_sequenceInfo{nullptr};
        QSharedPointer<AutoAnsThreadHandler> _autoAnsHandler;
};
