// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "models/apcanseqactioninfo.hpp"


/** @brief This is auto answer to send when an expacted message is received
    @note The method @ref parsePCanSequenceActionInfo makes the class data consistent */
class AutoAnsSequActionInfo : public APCanSeqActionInfo
{
    Q_OBJECT

    private:
        /** @brief Class constructor
            @param msgIdx This is the message index used by caller class, it's needed to link the
                          message to send with the expected message
            @param parent The parent instance */
        explicit AutoAnsSequActionInfo(quint8 msgIdx, QObject *parent = nullptr);

    public:
        /** @brief Class destructor */
        virtual ~AutoAnsSequActionInfo() override;

    public:
        /** @brief Get the message index used by caller class, it's needed to link the message to
                   send with the expected message */
        quint8 getMsgIdx() const { return _msgIdx; }

    public:
        /** @brief Creata a @ref AutoAnsSequActionInfo instance thanks to the parameters given
            @param cmd The stringified CAN command to parse
            @param hasBitrateSwitch The bitrate switch to apply to the command
            @param isExtendedFrameFormat The extended frame format to apply to the command
            @return The created @ref AutoAnsSequActionInfo thanks to the given parameters or
                    nullptr if a problem occurred. */
        static const AutoAnsSequActionInfo *parsePCanSequenceActionInfo(
            const QString &cmd,
            bool hasBitrateSwitch,
            bool isExtendedFrameFormat);

        /** @brief Extract the commands which are linked to the @ref msgIdx from the @ref allCmds
                   list; and add them into the @ref expectedCmds list.
            @note "Extract" means that we remove the elements from @ref allCmds
            @param msgIdx The message index used to get the verification on the right message
            @param allCmds The list from where we search and extract the commands
            @param expectedCmds The list to fill with the extracted commands */
        static void extractSequAction(int msgIdx,
                                      QVector<const AutoAnsSequActionInfo*> &allCmds,
                                      QVector<const AutoAnsSequActionInfo*> &expectedCmds);

    private:
        /** @brief Extract the message index from the extended stringify command, and extract the
                   stringified commandto be parsed by default CAN messager parser
            @param cmdExtended The command wich contains the stringified command and the message idx
            @param msgIdx The extract message index
            @param pureCmd The command extract from the extented format */
        static bool extractMsgIdxAndCmdStr(const QString &cmdExtended,
                                           quint8 &msgIdx,
                                           QString &pureCmd);

    private:
        /** @brief The number of extended command parts */
        static const constexpr int CmdPartsNb = 4;

        /** @brief This is the index of the message index in the extended command */
        static const constexpr int MsgIdxPartIdx = 0;

        /** @brief This is the start index of the command in the extended format */
        static const constexpr int PureCmdPartStartIdx = 1;

        /** @brief This is the end index of the command in the extended format */
        static const constexpr int PureCmdPartEndIdx = 3;

    private:
        quint8 _msgIdx{0};
};
