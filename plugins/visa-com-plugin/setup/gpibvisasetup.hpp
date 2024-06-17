// SPDX-FileCopyrightText: 2021 Damien Manceau <damien.manceau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "avisasetup.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module is used to setup a VISA communication using a GPIB interface */
class GpibVisaSetup : public AVisaSetup
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(GpibVisaSetup)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit GpibVisaSetup(QObject *parent = nullptr);

    protected:
        /** @see AVisaSetup::initParamsFormat
            @note @ref gpibNumberParamName : number (The device number used for gpib communication)
            @note @ref gpibAddressParamName : number (The gpib address of the gpib device) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see AVisaSetup::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @see AVisaSetup::createAndSetupItf */
        virtual QSharedPointer<AVisacom> createAndSetupItf() override;

        /** @brief Allow to get the GPIB number for each child class of GpibVisaSetup
            @return gpib number in quint16*/
        quint16 getGpibNumber();

        /** @brief Allow to get the GPIB address for each child class of GpibVisaSetup
            @return gpib address in quint16*/
        quint16 getGpibAddress();

    private:
        static const constexpr char *gpibNumberParamName = "gpibNumber";
        static const constexpr char *gpibNumberParamDesc = "The device number used for gpib "
                                                          "communication";

        static const constexpr char *gpibAddressParamName = "address";
        static const constexpr char *gpibAddressParamDesc = "The gpib address of the gpib device";

    private:
        quint16 _gpibNumber{0};
        quint16 _address{0};
};
