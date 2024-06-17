// SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/ajlinkintffromparams.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"


/** @brief The module flashes a board connected to a JLink probe. */
class JLinkFlash : public AJLinkIntfFromParams
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(JLinkFlash)

    public:
        /** @brief Class constructor
            @param parent The parent class */
        Q_INVOKABLE explicit JLinkFlash(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~JLinkFlash() override;

    protected:
        /** @see AJLinkInterface::initParamsFormat
            @note @ref DestAddressParamName : number (The binary destination address)
                  @ref EraseBeforeFlashParamName : boolean (If true, erase before flashing)
                  @ref StartCpuAfterFlashParamName : boolean (If true, the CPU board is started
                                                              after flashing) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc AJLinkInterface::initInputsFormat
            @note @ref InputBinPathName : string (The absolute path of the binary to flash) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @copydoc AJLinkInterface::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @copydoc AJLinkIntfFromParams::mainJLinkProcess */
        virtual bool mainJLinkProcess(const JsonArray &jsonObjectInputValues,
                                      const QString &jLinkPgmPath,
                                      JsonArray &output) override;

    private:
        /** @brief Call to retrieve inputValues and execute JLink.exe
            @param inputValues The entry values for the flashing
            @param jLinkPgmPath The JLink program path
            @return True if no problem occurred */
       bool operate(const JsonArray &inputValues,
                     const QString &jLinkPgmPath);

    private:
        static const constexpr char *InputBinPathName = "binPath";
        static const constexpr char *InputBinPathDesc = "The absolute path of the binary to flash";

        static const constexpr char *DestAddressParamName = "flashingDestAddress";
        static const constexpr char *DestAddressParamDesc = "Flashing destination address";

        static const constexpr char *EraseBeforeFlashParamName = "eraseBeforeFlash";
        static const constexpr char *EraseBeforeFlashParamDesc = "Tick to erase the board before "
                                                                 "flashing";

        static const constexpr char *StartCpuAfterFlashParamName = "startCpuAfterFlash";
        static const constexpr char *StartCpuAfterFlashParamDesc = "Tick to start the CPU after "
                                                                   "flashing";

        static const constexpr char *HmiName = "Flash board";
        static const constexpr char *HmiDescription = QT_TR_NOOP("(TR) Board is being flashed, "
                                                                 "this can take a few seconds");

        /** @brief Flashing address hexadecimal base */
        static const constexpr int HexBase = 16;

        /** @brief Width of the flashing address field */
        static const constexpr int FlashingAddressFieldWidth = 8;

    private:
        quint32 _flashingAddress {0};
        bool _eraseBeforeFlash{false};
        bool _startCpuAfterFlash{false};
};
