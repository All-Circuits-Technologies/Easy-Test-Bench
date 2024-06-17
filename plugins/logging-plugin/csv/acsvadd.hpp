// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"


/** @brief Abstract class to manage the adding of elements in a CSV line */
class ACsvAdd : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The module class name
            @param parametersFormat The format of the module parameters
            @param parent The parent instance class */
        explicit ACsvAdd(const QString &moduleClassName,
                         QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ACsvAdd() override = default;

    protected slots:
        /** @brief DefaultSequenceInstanceModule::processModule */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @brief This method is called to append data to the CSV line
            @param jsonObjectInputValues The input values got
            @param csvLine The CSV line to append (the list already contains the data previously
                           added
            @return True if no problem occurs */
        virtual bool append(const JsonArray &jsonObjectInputValues,
                            QVector<QString> &csvLine) = 0;

        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputCsvLineName : array<string> (The list represents the column of the CSV
                                                         line; therefore this is the line to append
                                                         with data) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref outputCsvLineName : array<string> (The list represents the column of the CSV
                                                          line; therefore this is the line with the
                                                          data added)*/
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

    private:
        static const constexpr char *InputCsvLineName = "csvLine";
        static const constexpr char *InputCsvLineDesc = "CSV line to append with data";

        static const constexpr char *OutputCsvLineName = "outCsvLine";
        static const constexpr char *OutputCsvLineDesc = "The CSV line with the data append";
};
