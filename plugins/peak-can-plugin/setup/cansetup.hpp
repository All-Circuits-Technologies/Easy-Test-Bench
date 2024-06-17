// SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "setup/acansetup.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module is used to setup a CAN communication */
class CanSetup : public ACanSetup
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(CanSetup)

    public:
    /** @brief Class constructor
            @param parent The object parent */
        Q_INVOKABLE explicit CanSetup(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CanSetup() override;

    protected:
        /** @copydoc ACanSetup::initParamsFormat
            @see ACanSetup::addCommonParamsFormat
            @note @ref BaudRateParamName : number (This is the CAN baud rate, to set) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @copydoc DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    private:
        static const constexpr char *BaudRateParamName = "baudRate";
        static const constexpr char *BaudRateParamDesc = "This is the CAN baud rate, to set";
};
