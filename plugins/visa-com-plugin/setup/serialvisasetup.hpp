// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "avisasetup.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "qtvisacomlib/src/serial/flowcontrol.hpp"
#include "qtvisacomlib/src/serial/parity.hpp"
#include "qtvisacomlib/src/serial/stopbits.hpp"


class VisacomAsrl;


/** @brief This module is used to setup a VISA communication using an SERIAL interface */
class SerialVisaSetup : public AVisaSetup
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(SerialVisaSetup)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit SerialVisaSetup(QObject *parent = nullptr);

    protected:
        /** @see AVisaSetup::initParamsFormat
            @note @ref comPortParamName : number (The COM port used for serial communication)
            @note @ref baudrateParamName : number (The baudrate value of serial device)
            @note @ref databitsParamName : number (The databits value of serial device)
            @note @ref flowControlParamName : string (The flowControl of serial device)
            @note @ref parityParamName : string (The parity value of serial device)
            @note @ref stopBitsParamName : string (The stopBits value of serial device) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see AVisaSetup::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @see AVisaSetup::createAndSetupItf */
        virtual QSharedPointer<AVisacom> createAndSetupItf() override;

    private:
        /** @brief Manage the enum selection from file test sequence
            @note This function manage the FlowControl | Parity | StopBits Enum
            @param parameters The format of the module parameters, which is possible to set
            @param paramName The name of the object searched
            @param errors The array to store errors
            @param enumTypeStr The enum selected in file sequence converted to QString
            @return return a boolean value. if false => An error occured */
        bool getEnumSelection(const JsonArray &parameters,
                              const char *paramName,
                              JsonArray &errors,
                              QString &enumTypeStr);

    private:
        static const constexpr char *comPortParamName = "comPort";
        static const constexpr char *comPortParamDesc = "The COM port used for serial"
                                                        "communication";

        static const constexpr char *baudrateParamName = "baudrate";
        static const constexpr char *baudrateParamDesc = "The baudrate value of serial device";

        static const constexpr char *databitsParamName = "databits";
        static const constexpr char *databitsParamDesc = "The databits value of serial device";

        static const constexpr char *flowControlParamName = "flowcontrol";
        static const constexpr char *flowControlParamDesc = "The flowControl value of serial device";

        static const constexpr char *parityParamName = "parity";
        static const constexpr char *parityParamDesc = "The parity value of serial device";

        static const constexpr char *stopBitsParamName = "stopbits";
        static const constexpr char *stopBitsParamDesc = "The stopBits value of serial device";


        static const constexpr quint32 minBaudrate = 75;
        static const constexpr quint32 maxBaudrate = 230400;
        static const constexpr quint32 defaultBaudrate = 9600;

        static const constexpr quint32 minDatabits = 5;
        static const constexpr quint32 maxDatabits = 8;

    private:
        quint16 _comPort{0};
        quint32 _baudrate{defaultBaudrate};
        quint16 _databits{maxDatabits};

        FlowControl::Enum _flowControl{ FlowControl::NoFlowControl };
        Parity::Enum _parity{ Parity::NoParity };
        StopBits::Enum _stopBits{ StopBits::One };

        QSharedPointer<VisacomAsrl> _visacomasrl;
};
