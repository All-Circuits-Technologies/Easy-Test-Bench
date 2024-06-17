// SPDX-FileCopyrightText: 2019 - 2020 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QRegularExpression>


/** @brief This namespace contains serial-link plugin lib constants */
namespace SerialPluginLibConstants
{
    namespace debug
    {
        /** @brief Should serial plugin print useful hints in debug channel
            @note Developers may want to set it to true while working on this plugin  */

#ifdef ACT_BUILD_RELEASE
        constexpr const bool Hints = false;
#else
        constexpr const bool Hints = true;
#endif

        /** @brief Should serial activity be printed on debug channel
            @note Developers may want to set it to true while working on this plugin */
#ifdef ACT_BUILD_RELEASE
        constexpr const bool Stream = false;
#else
        constexpr const bool Stream = true;
#endif

        /** @brief Should serial plugin be verbose about its internal work
            @note Developers may want to set it to true while working on this plugin  */
#ifdef ACT_BUILD_RELEASE
        constexpr const bool Internals = false;
#else
        constexpr const bool Internals = true;
#endif
    }

    /** @brief JSON names
        @note An effort is put to re-use names accross modules */
    namespace Json
    {
        constexpr const char *SerialNumber = "serial-number";
        constexpr const char *UsbVid = "usb-vid";
        constexpr const char *UsbPid = "usb-pid";
        constexpr const char *Description = "description";
        constexpr const char *Manufacturer = "manufacturer";
        constexpr const char *Location = "location";
        constexpr const char *Found = "found";
        constexpr const char *Baudrate = "baudrate";
        constexpr const char *SwFlowCtl = "sw-flow-ctl";
        constexpr const char *HwFlowCtl = "hw-flow-ctl";
        constexpr const char *ExpectedPattern = "expected-pattern";
        constexpr const char *FailurePattern = "failure-pattern";
        constexpr const char *Timeout = "timeout-ms";
        constexpr const char *Message = "message";
        constexpr const char *CapturedGroup = "captured-group";
        constexpr const char *Mandatory = "mandatory";
        constexpr const char *EmitErrorIfFail = "exitIfFail";

        constexpr const char *NthElement = "nthElement";
        constexpr const char *PortName = "portName";
        constexpr const char *PortNameDesc = "Serial port name (ex: COM1)";
        constexpr const char *EndOfLine = "end-of-line";
        constexpr const char *EndOfLineDesc = "End of line string (defaults to LF)";
        constexpr const char *Trim = "trim";
        constexpr const char *TrimDesc = "Should received lines be trimmed before verifications";
        constexpr const char *IsMultiLineTest = "isMultiLineTest";
        constexpr const char *IsMultiLineTestDesc = "If true, the expected regexp pattern will be "
                                                    "tested with all the received data."
                                                    "If false, the regexp pattern will only be "
                                                    "tested with each line.";
        constexpr const char *ForceFlush = "forceFlush";
        constexpr const char *ForceFlushDesc = "If true, we force flushing after having written "
                                               "data to serial port.";
        constexpr const char *OneCapturingRegExpDesc = "This is the pattern of regular expression "
                                                       "to extract the measure value from the "
                                                       "script logs. To extract properly the "
                                                       "value, you have to set only one capturing "
                                                       "group with parenthesis \"()\".";
    }

    namespace Defaults
    {
        constexpr const int Baudrate = 9600;
        constexpr const bool SwFlowCtl = false;
        constexpr const bool HwFlowCtl = false;
        static const QRegularExpression DefaultExpectedFailure = QRegularExpression(QString());

        constexpr const bool Trim = true;
        constexpr const bool MultiLine = false;
        constexpr const char* EndOfLine = "\n";
        constexpr const quint32 NthElement = 0;
        constexpr const bool ForceFlush = false;
        const constexpr char *OneCapturingParamRegExp =
            R"(^(?>(?>\(\?>)|[^(\r\n])*\((?>(?>\(\?>)|[^(\r\n])*\)(?>(?>\(\?>)|[^(\r\n])*$)";
    }

    /** @brief This defines constants for parsing process */
    namespace Parser
    {
        /** @brief The character used to separate the different parts in the expected messages */
        const constexpr char StrSeparator = '/';
    }
}
