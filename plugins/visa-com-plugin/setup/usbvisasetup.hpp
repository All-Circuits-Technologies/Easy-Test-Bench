// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "avisasetup.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module is used to setup a VISA communication using an USB interface */
class UsbVisaSetup : public AVisaSetup
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(UsbVisaSetup)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit UsbVisaSetup(QObject *parent = nullptr);

    protected:
        /** @see AVisaSetup::initParamsFormat
            @note @ref usbNumberParamName : number (The device number used for usb communication)
            @note @ref vendorIdParamName : number (The vendor id of the usb device)
            @note @ref productIdParamName : number (The product id of the usb device)
            @note @ref serialNumberParamName : string (The serial number of the usb device)
            @note @ref itfNumberParamName : number (The interface number of the usb device) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see AVisaSetup::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @see AVisaSetup::createAndSetupItf */
        virtual QSharedPointer<AVisacom> createAndSetupItf() override;

    private:
        static const constexpr char *usbNumberParamName = "usbNumber";
        static const constexpr char *usbNumberParamDesc = "The device number used for usb "
                                                          "communication";

        static const constexpr char *vendorIdParamName = "vid";
        static const constexpr char *vendorIdParamDesc = "The vendor id of the usb device";

        static const constexpr char *productIdParamName = "pid";
        static const constexpr char *productIdParamDesc = "The product id of the usb device";

        static const constexpr char *serialNumberParamName = "serialNumber";
        static const constexpr char *serialNumberParamDesc = "The serial number of the usb device";

        static const constexpr char *itfNumberParamName = "interfaceNumber";
        static const constexpr char *itfNumberParamDesc = "The interface number of the usb device";

    private:
        quint16 _usbNumber{0};
        quint32 _vid{0};
        quint32 _pid{0};
        QString _serialNumber;
        quint16 _itfNumber{0};
};
