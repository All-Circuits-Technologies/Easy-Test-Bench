// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/aatmelprogramitf.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"

class TemporaryFile;


/** @brief Read the microprocessor EEPROM and write the result to a linked file.
    @note At each call of process module, the instance removes the previous read file linked (if it
          exists).
    @note When the instance is destroyed the read file linked is destroyed */
class ReadEepromAtProgram : public AAtmelProgramItf
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(ReadEepromAtProgram)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit ReadEepromAtProgram(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ReadEepromAtProgram() override;

    protected:
        /** @see DefaultSequenceInstanceModule::initOutputsFormat
            @note @ref outputReadFileName : string (Path to the Intel hex file which contains the
                                                    binaries read) */
        virtual bool initOutputsFormat(JsonArray &outputsFormat) const override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule
            @note The module display a view to tell the user that we are waiting for the board */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    private:
        /** @brief Process the read feature
            @param jsonObjectInputValues The input object values to parse
            @return True if no problem occurs */
        bool readProcess(const JsonArray &jsonObjectInputValues);

        /** @brief Manage the construction and destruction of the temporary file linked to the
                   reading
            @return True if no problem occurs */
        bool manageTemporaryFile();

    private:
        static const constexpr char *HmiName = "Read EEPROM";
        static const constexpr char *HmiDescription = QT_TR_NOOP("(TR) We are reading the EEPROM, "
                                                                 "this can take a few seconds");

        static const constexpr char *OutputReadFileName = "hexFilePath";
        static const constexpr char *OutputReadFileDescription = "Path to the Intel hex file which "
                                                                 "contains the binaries read";

        static const constexpr int DefaultReadTimeoutInMs = 1500;

        static const constexpr char *HexFileBaseName = "tb_readFile";

    private:
        TemporaryFile *_readInEepromFile{nullptr};
};
