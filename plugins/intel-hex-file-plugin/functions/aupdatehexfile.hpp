// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"


/** @brief Abstract class to update an existing hex file */
class AUpdateHexFile : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The module class name
            @param parent The parent instance class */
        explicit AUpdateHexFile(const QString &moduleClassName,
                                QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AUpdateHexFile() override = default;

    protected:
        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputIntelHexFilePathName : string (The path of the intel hex file to u
                                                           pdate) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

    protected:
        /** @brief Call to update the data contains in a hex file
            @param jsonObjectInputValues The module input values
            @param address The address where to set the data
            @param value The value to write in the hex file
            @return True if no problem occurs */
        bool updateHexProcess(const JsonArray &jsonObjectInputValues,
                              quint16 address,
                              const QByteArray &value);

    private:
        static const constexpr char * InputIntelHexFilePathName = "hexFilePath";
        static const constexpr char * InputIntelHexFilePathDesc = "The path of the intel hex file "
                                                                  "to update";
};
