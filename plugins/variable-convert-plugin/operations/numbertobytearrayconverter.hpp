// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "endianesstype.hpp"
#include "numberutility/number.hpp"


/** @brief Useful module for converting a number to a qbytearray
    @note Because the QByteArray doesn't exist yet, this will output a string representation of
          the byte array */
class NumberToByteArrayConverter : public DefaultSequenceInstanceModule
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(NumberToByteArrayConverter)

    public:
        /** @brief Class constructor
            @param parent Instance parent class */
        Q_INVOKABLE explicit NumberToByteArrayConverter(QObject *parent = nullptr);

    protected:
        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref endianessParamName : array<string> (Choose the endianess of the output byte
                                                           array)
                  @ref byteSizeParamName : number (Choose the final byte size)
                  @ref powerOfTenToApplyBeforeConvertName : number (Apply a power of ten before
                                                                    converting the byte array) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputNumberName : number (The input contains the number to convert) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref outputByteArrayName : string (This output contains the converted byte
                                                     array) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule
            @note The module display a view to tell the user that we are waiting for the board */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Convert the given number to a byte array
            @param jsonObjectInputValues The inputs to get the number from
            @param output The output to contain the byte array converted (in string)
            @return True if no problem occurs */
        bool convert(const JsonArray &jsonObjectInputValues, JsonArray &output);

        /** @brief Convert the number to a byte array
            @param value The number to convert
            @param result The number converted to byte array */
        bool convertToByteArray(const Number &value, QByteArray &result);

    private:
        static const constexpr char *endianessParamName = "endianess";
        static const constexpr char *endianessParamDesc = "Choose the endianess of the output "
                                                          "byte array";

        static const constexpr char *byteSizeParamName = "byteSize";
        static const constexpr char *byteSizeParamDesc = "Choose the final byte size";

        static const constexpr char *powerOfTenToApplyBeforeConvertName = "powerOfTenToApply";
        static const constexpr char *powerOfTenToApplyBeforeConvertDesc = "Apply a power of ten "
                                                                          "before converting the "
                                                                          "byte array";

        static const constexpr char *outputByteArrayName = "outputByteArray";
        static const constexpr char *outputByteArrayDesc = "This output contains the converted "
                                                           "byte array";

        static const constexpr char *inputNumberName = "inputNumber";
        static const constexpr char *inputNumberDesc = "The input contains the number to convert";

        static const constexpr int defaultPowerOfTenToApply = 0;

        static const constexpr int maxPowerOfTen = 8;

    private:
        quint8 _byteSize{1};
        EndianessType::Enum _endianess{EndianessType::Unknown};
        qint8 _powerOfTen{0};
};
