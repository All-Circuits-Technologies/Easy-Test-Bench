// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "acsvadd.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module appends a number to the string list given, which is the representation of a
           CSV line. */
class CsvAddNumber : public ACsvAdd
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(CsvAddNumber)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit CsvAddNumber(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CsvAddNumber() override;

    protected:
        /** @see ACsvAdd::append */
        virtual bool append(const JsonArray &jsonObjectInputValues,
                            QVector<QString> &csvLine) override;

        /** @see ACsvAdd::initInputsFormat
            @note @ref inputNumberToAddName : number (The number to add to the CSV line) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    private:
        static const constexpr char *InputNumberToAddName = "numberToAdd";
        static const constexpr char *InputNumberToAddDesc = "This is the number to add in the CSV "
                                                            "line";
};
