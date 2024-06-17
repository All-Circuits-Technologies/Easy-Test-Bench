// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "abstractpluginelements/defaultsequenceinstancemodule.hpp"

#include "numberutility/number.hpp"

#include "types/atprogramphysicalitftype.hpp"


/** @brief Abstract Atprogram interface used to communicate with the atprogram */
class AAtmelProgramItf : public DefaultSequenceInstanceModule
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The module class name
            @param parent The parent instance class */
        explicit AAtmelProgramItf(const QString &moduleClassName,
                                  QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AAtmelProgramItf() override = default;

    protected:
        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref physicalItfParamName : string (The physical interface used to communicate
                                                      with the board)
                  @ref chipUsedParamName : string (The name of the chip to communicate with)
                  @ref timeoutMsParamName : number (Read timeout in Ms) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::initInputsFormat
            @note @ref inputAtPgmExePathName : string (The atprogram absolute executable path) */
        virtual bool initInputsFormat(JsonArray &inputsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief Call the atprogram executable with the given arguments
            @note If the parameter processExitProperly is null, the method will return false if the
                    process doesn't exit properly
                  If the parameter processExitProperly isn't null, the method will return true if
                    the process doesn't exit properly and the param value will be equal to false
            @param atProgramPath The path to the atprogram executable
            @param arguments The arguments to pass to the program
            @param logProcessError If true and if an error occured, they will be written in logs
            @param overrideTimeoutInMs This param is used to override the timeout sets by the user
                                       (if not equals to -1)
            @param processExitProperly If not null, this will say if the process exit properly,
                                       the param is equals to false except when the process succeeds
                                       and no error occurs
            @return True if no problem occured */
        bool callProcess(const QString &atProgramPath,
                         const QStringList &arguments,
                         bool logProcessError = true,
                         int overrideTimeoutInMs = -1,
                         bool *processExitProperly = nullptr);

        /** @brief Get the timeout sets by the user */
        int getTimeoutInMs() const { return _timeoutInMs; }

        /** @brief Parse the input object values to get the AT program path
            @param jsonObjectInputValues The input object values to parse
            @param atProgramPath The AT program path got
            @return True if no problem occurs */
        bool parseAtProgramPath(const JsonArray &jsonObjectInputValues, QString &atProgramPath);

    protected:
        /** @brief Get the format of parameters managed by this module */
        static bool getSharedParams(JsonArray &sharedParams);

    protected:
        /** @brief arguments to pass to the commands in the arguments section,
                     Format: atprogram [options] <command> <arguments>
            @note From microchip doc:
                    --verify: Verify memory after programming. */
        static const constexpr char* AtPgmVerifyArg = "--verify";

        /** @brief arguments to pass to the  infocommands in the arguments section,
                     Format: atprogram [options] <command> <arguments>
            @note From microchip doc:
                    --voltage:  Display the target voltage only. */
        static const constexpr char* AtPgmVoltageArg = "--voltage";

        /** @brief Options to pass to the atprogram in the options section,
                     Format: atprogram [options] <command> <arguments>
            @note From microchip doc:
                    -f --file (file): File to be programmed or where the data are read. Intel® .hex,
                                      .elf, or binary. */
        static const constexpr char* AtPgmFileArg = "-f";

        /** @brief arguments to pass to the commands in the arguments section,
                     Format: atprogram [options] <command> <arguments>
            @note From microchip doc:
                    --tool <arg>: Tool name: Atmel-ICE, AVR Dragon™, AVR ISP mkII, AVR ONE,
                                  JTAG ICE3, JTAGICE mkII, QT™600, STK500, STK600, SAM-ICE™, EDBG,
                                  MEDBG, Power Debugger, Mega DFU orFLIP. */
        static const constexpr char* AtPgmDbgArg = "-t";

        /** @brief Default tool name used in the app, we use the Atmel ICE */
        static const constexpr char* AtPgmDefaultDbg = "atmelice";

        /** @brief arguments to pass to the commands in the arguments section,
                     Format: atprogram [options] <command> <arguments>
            @note From microchip doc:
                    --interface <arg>: Physical interface: aWire, DebugWIRE, HVPP, HVSP, ISP, JTAG,
                                       PDI, UPDI,TPI, or SWD. */
        static const constexpr char* AtPgmItfArg = "-i";

        /** @brief arguments to pass to the commands in the arguments section,
                     Format: atprogram [options] <command> <arguments>
            @note From microchip doc:
                     --device <arg>: Device name. E.g. ATxmega128A1 or AT32UC3A0256 */
        static const constexpr char* AtPgmChipUsedArg = "-d";

    private:
        static const constexpr char *InputAtPgmExePathName = "atProgramExe";
        static const constexpr char *InputAtPgmExePathDesc = "The atprogram absolute executable "
                                                             "path";

        static const constexpr char *TimeoutMsParamName = "timeoutInMs";
        static const constexpr char *TimeoutMsParamDesc = "Program timeout in Ms";

        static const constexpr char *PhysicalItfParamName = "physicalInterface";
        static const constexpr char *PhysicalItfParamDesc = "The physical interface used to "
                                                            "communicate with the board";

        static const constexpr char *ChipUsedParamName = "chipUsedName";
        static const constexpr char *ChipUsedParamDesc = "The name of the chip targetted by the "
                                                         "action";

        /** @brief Character number limit, in order to not display all the output of JLink output
                   to debug, only the last char */
        static const constexpr int ProcessLogCharLimitToDisplay { 900 };

        static const constexpr int DefaultTimeoutInMs = 1500;

    private:
        int _timeoutInMs{ DefaultTimeoutInMs };
        AtProgramPhysicalItfType::Enum _itfType{ AtProgramPhysicalItfType::Unknown };
        QString _chipName;
};
