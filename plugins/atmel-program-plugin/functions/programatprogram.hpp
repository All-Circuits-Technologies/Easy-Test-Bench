// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/aatmelprogramitf.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"
#include "numberutility/number.hpp"

#include "types/memorytype.hpp"


/** @brief Program the microprocessor memory with a hex file through the atmel probe. */
class ProgramAtProgram : public AAtmelProgramItf
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(ProgramAtProgram)

    public:
        /** @brief Class constructor
            @param parent The parent instance class */
        Q_INVOKABLE explicit ProgramAtProgram(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ProgramAtProgram() override;

    protected slots:
        /** @see DefaultSequenceInstanceModule::processModule
            @note The module display a view to tell the user that we are waiting for the board */
        virtual void processModule(const JsonArray &jsonObjectInputValues) override;

    protected:
        /** @see AAtmelProgramItf::initInputsFormat
            @note @ref inputReadFileName : string (Path to the Intel hex file which contains the
                                                   binaries)  */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see AAtmelProgramItf::initParamsFormat
            @note @ref verifyAfterProgramParamName : boolean (The module will verify the memory
                                                              after program)
                  @ref deleteFileAfterProgramParamName : boolean (The module will Delete the input
                                                                  file after programming the
                                                                  memory)
                  @ref memoryTypeParamName : array<string> (Allow to choose the memory type to
                                                            manage) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see AUpdateHexFile::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

    private:
        /** @brief Process the write feature
            @return True if no problem occurs */
        bool writeProcess(const JsonArray &jsonObjectInputValues);

    private:
        static const constexpr char *VerifyAfterProgramParamName = "verifyAfterProgram";
        static const constexpr char *VerifyAfterProgramParamDesc = "The module will verify the "
                                                                   "memory after program";

        static const constexpr char *DeleteFileAfterProgramParamName = "deleteFileAfterProgram";
        static const constexpr char *DeleteFileAfterProgramParamDesc = "Delete the input file "
                                                                       "after programming the "
                                                                       "memory";

        static const constexpr char *MemoryTypeParamName = "memoryType";
        static const constexpr char *MemoryTypeParamDesc = "Allow to choose the memory type to "
                                                           "manage";

        static const constexpr char *HmiName = "Program Memory";
        static const constexpr char *HmiDescription = QT_TR_NOOP("(TR) We are updating the memory, "
                                                                 "this can take a few seconds");

        static const constexpr char *InputReadFileName = "hexFilePath";
        static const constexpr char *InputReadFileDescription = "Path to the Intel hex file which "
                                                                 "contains the binaries to write";

        static const constexpr bool DefaultValueVerifyAfterProgram = true;
        static const constexpr bool DefaultDeleteFileAfterProgram = false;

    private:
        bool _verifyAfterWriting{DefaultValueVerifyAfterProgram};
        bool _deleteFileAfterProgram{DefaultDeleteFileAfterProgram};
        MemoryType::Enum _memoryType{ MemoryType::Unknown };
};
