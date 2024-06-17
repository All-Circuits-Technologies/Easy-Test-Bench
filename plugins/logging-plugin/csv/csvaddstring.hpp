// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "acsvadd.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module appends a string to the string list given, which is the representation of a
           CSV line. */
class CsvAddString : public ACsvAdd
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(CsvAddString)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit CsvAddString(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~CsvAddString() override;

    protected:
        /** @see ACsvAdd::append */
        virtual bool append(const JsonArray &jsonObjectInputValues,
                            QVector<QString> &csvLine) override;

        /** @see ACsvAdd::initInputsFormat
            @note @ref inputStringToAddName : string (The string to add to the CSV line) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    private:
        static const constexpr char *InputStringToAddName = "stringToAdd";
        static const constexpr char *InputStringToAddDesc = "This is the string to add in the CSV "
                                                            "line";
};
