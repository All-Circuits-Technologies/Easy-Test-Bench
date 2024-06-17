// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/aupdatehexfile.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module is useful to update the content of a hex file
    @note The address is given by parameter and the value by input */
class UpdateHexFile : public AUpdateHexFile
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(UpdateHexFile)

    public:
        /** @brief Class constructor
            @param parent The parent class instance */
        Q_INVOKABLE explicit UpdateHexFile(QObject *parent = nullptr);

    public:
        /** @see AUpdateHexFile::initInputsFormat
            @note @ref inputDataValueName : string (This is the data to update on the hex file. The
                                                    date has to be a hexadecimal number)*/
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see AUpdateHexFile::initParamsFormat
            @note @ref addressParamName : number (The data is updated from this address)*/
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @see AUpdateHexFile::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    private:
        /** @brief Useful to process the update function
            @param jsonObjectInputValues The module input values
            @return True if no problem occurs */
        bool processUpdate(const JsonArray &jsonObjectInputValues);

    private:
        static const constexpr char *AddressParamName = "addressToUpdate";
        static const constexpr char *AddressParamDesc = "The data is updated from this address";

        static const constexpr char *InputDataValueName = "dataValueToUpdate";
        static const constexpr char *InputDataValueDesc = "This is the data to update on the hex "
                                                          "file. The date has to be a hexadecimal "
                                                          "number";

    private:
        quint16 _address{0};
};
