// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "functions/ajlinkinterface.hpp"

#include "numberutility/number.hpp"

class QFile;


/** @brief Abstract class to use JLink.exe for a card given */
class AJLinkIntfFromParams : public AJLinkInterface
{
    Q_OBJECT

    public:
        /** @brief Class constructor
            @param moduleClassName The module class name
            @param parent The class parent */
        explicit AJLinkIntfFromParams(const QString &moduleClassName, QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~AJLinkIntfFromParams() override;

    protected:
        /** @see DefaultSequenceInstanceModule::initParamsFormat
            @note @ref DeviceParamName : string (Selects the target device.)
                  @ref InterfaceParamName : string (Configures the target interface.)
                  @ref SpeedParamName : number (Configures the target interface speed.) */
        virtual bool initParamsFormat(JsonArray &paramsFormat) const override;

        /** @see DefaultSequenceInstanceModule::validateAndSetParameters */
        virtual bool validateAndSetParameters(const JsonArray &parameters,
                                              JsonArray &errors) override;

        /** @brief Call JLink in a process
            @attention By default, the function adds the "eoe 1" command at start (if it isn't
                       already in the commands list given) this command means:
                            if a command fails, JLink.exe exits in error.
            @note If the parameter processExitProperly is null, the method will return false if the
                    process doesn't exit properly
                  If the parameter processExitProperly isn't null, the method will return true if
                    the process doesn't exit properly and the param value will be equal to false
            @param commands The commands to add in temporary file, in order to be called by JLink
            @param timeoutInMs The process execution timeout, if -1 the timeout is disabled
            @param logProcessError If true and if an error occured, they will be written in logs
            @param processExitProperly If not null, this will say if the process exit properly,
                                       the param is equals to false except when the process succeeds
                                       and no error occurs
            @return True if no problem occurred */
        bool callCmdsProcess(const QStringList &commands,
                             const QString &jLinkPgmPath,
                             int timeoutInMs = -1,
                             bool logProcessError = true,
                             bool *processExitProperly = nullptr);

    private:
        /** @brief Appends the communication commands got from the module parameters to the
                   commands list given.
            @param commands The commands list to append */
        void appendCommunicationCmds(QStringList &commands);

    protected:
        /** @brief From JLink documentation: "Select specific device J-Link shall connect to"
            @note Syntax:
                    Device <DeviceName>

                    +------------+---------------------------------------------------+
                    | Parameter  |                     Meaning                       |
                    +------------+---------------------------------------------------+
                    | DeviceName | Valid device name: Device is selected.            |
                    |            | ?: Shows device selection dialog.                 |
                    +------------+---------------------------------------------------+
            @note Example:
                    Device STM32F407VE */
        static const constexpr char *JLinkCmdDevice = "device %1";

        /** @brief From JLink documentation: "Select target interface."
            @note Syntax:
                    SelectInterface <Interface>

                    +------------+---------------------------------------------------+
                    | Parameter  |                     Meaning                       |
                    +------------+---------------------------------------------------+
                    | Interface  | Any supported target interface (e.g SWD, JTAG,    |
                    |            | ICSP, FINE, ...)                                  |
                    +------------+---------------------------------------------------+
            @note Example:
                    SelectInterface SWD */
        static const constexpr char *JLinkCmdSelectInterface = "si %1";

        /** @brief From JLink documentation: "Set target interface speed."
            @note Syntax:
                    Speed <Setting>

                    +------------+---------------------------------------------------+
                    | Parameter  |                     Meaning                       |
                    +------------+---------------------------------------------------+
                    |            | Decimal value: Interface speed in kHz.            |
                    | Setting    | auto: Interface speed is selected automatically.  |
                    |            | adaptive: Interface speed is set to adaptive.     |
                    +------------+---------------------------------------------------+
            @note Example:
                    Speed 4000 */
        static const constexpr char *JLinkCmdSpeed = "speed %1";

        /** @brief Allow to connect to the board
            @note Syntax:
                     connect
            @note Example:
                    connect */
        static const constexpr char *JLinkCmdConnect = "connect";

        /** @brief From JLink documentation: "Commander exits after error."
            @note Syntax:
                     ExitOnError <1|0>

                     +-----------+---------------------------------------------------+
                     | Parameter |                     Meaning                       |
                     +-----------+---------------------------------------------------+
                     |    1/0    | 1: J-Link Commander will now exit on Error.       |
                     |           | 0: J-Link Commander will no longer exit on Error. |
                     +-----------+---------------------------------------------------+
            @note Example:
                    eoe 1 */
        static const constexpr char *JLinkCmdEoeEnable = "eoe 1";

        /** @brief From JLink documentation:
                    "This command programs a given data file to a specified destination address.
                    Currently supported data files are: *.mot, *.srec, *.s19, *.s, *.hex, *.bin"
            @note Syntax:
                     loadfile <Filename> [<Addr>]

                     +------------------------------------------------+-----------+
                     | Parameter                                      |  Meaning  |
                     +------------------------------------------------+-----------+
                     | Source filename                                |           |
                     +------------------------------------------------+-----------+
                     | Destination address (Required for *.bin files) |           |
                     +------------------------------------------------+-----------+
            @note Example:
                    loadfile C:/Work/test.bin 0x20000000 */
        static const constexpr char *JLinkCmdLoadFile = "loadfile \"%1\" %2";

        /** @brief From JLink documentation: "Resets and halts the target. "
            @note Syntax:
                    r */
        static const constexpr char *JLinkCmdResetAndHalts = "r";

        /** @brief From JLink documentation:
                    "Starts the CPU. In order to avoid setting breakpoints it allows to define a
                    maximum number of instructions which can be simulated/emulated. This is
                    particularly useful when the program is located in flash and flash breakpoints
                    are used. Simulating instructions avoids to reprogram the flash and speeds up
                    (single) stepping."
            @note Syntax:
                     go [<NumSteps> [<Flags>]]

                     +---------- +----------------------------------------------------------+
                     | Parameter |                     Meaning                              |
                     +-----------+----------------------------------------------------------+
                     |           | Maximum number of instructions allowed to be simulated.  |
                     | NumSteps  | Instruction simulation stops whenever a breakpointed     |
                     |           | instruction is hit, an instruction which cannot be       |
                     |           | simulated/emulated is hit or when NumSteps is reached.   |
                     +-----------+----------------------------------------------------------+
                     | Flags     | 0: Do not start the CPU if a BP is in range of NumSteps  |
                     |           | 1: Overstep BPs                                          |
                     +-----------+----------------------------------------------------------+
            @note Example:
                    go //Simply starts the CPU
                    go 20, 1 */
        static const constexpr char *JLinkCmdCpuStart = "go";

        /** @brief From JLink documentation:
                    "This command closes the target connection, the connection to the J-Link and
                    exits J-Link Commander."
            @note Syntax:
                    q */
        static const constexpr char *JLinkCmdExit = "exit";

        /** @brief From JLink documentation:
                    "Close JLink connection and quit"
            @note Syntax:
                    qc */
        static const constexpr char *JLinkCmdCloseAndQuit = "qc";

        /** @brief From JLink documentation:
                    "Erases all flash sectors of the current device. A device has to be specified
                    previously."
            @note Syntax:
                    erase */
        static const constexpr char *JLinkCmdErase = "erase";

        /** @brief From JLink documentation:
                    "Measure reaction time of RTCK pin."
            @note Syntax:
                mr [<RepCount>]

                +---------- +----------------------------------------------------------+
                | Parameter |                     Meaning                              |
                +-----------+----------------------------------------------------------+
                | RepCount  | Number of times the test is repeated (Default: 1).       |
                +-----------+----------------------------------------------------------+
            @note Example:
                    mr 3 */
        static const constexpr char *JLinkCmdMr = "mr %1";

    private:
        /** @brief Contains header commands to add at start of the commands list */
        static const QStringList CmdsListHeader;

    private:
        static const constexpr char *DeviceParamName = "device";
        static const constexpr char *DeviceParamDesc = "Selects the target device.";

        static const constexpr char *InterfaceParamName = "interface";
        static const constexpr char *InterfaceParamDesc = "Configures the target interface.";

        static const constexpr char *SpeedParamName = "speed";
        static const constexpr char *SpeedParamDesc = "Configures the target interface speed.";

        /** @brief This is the index in the commands list where to add the device cmd */
        static const constexpr int DeviceCmdLineIdxInFile = 1;

        /** @brief This is the index in the commands list where to add the select interface cmd */
        static const constexpr int SelectInterfaceCmdLineIdxInFile = 2;

        /** @brief This is the index in the commands list where to add the speed cmd */
        static const constexpr int SpeedCmdLineIdxInFile = 3;

    private:
        QString _valueDevice;
        QString _valueInterface;
        Number _valueSpeed;
};
