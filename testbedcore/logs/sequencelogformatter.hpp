// SPDX-FileCopyrightText: 2019 - 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "logs/sequencelogdata.hpp"
#include "logs/sequencelogformattype.hpp"
#include "numberutility/number.hpp"


/** @brief Helper methods to simplify the creation of sequence logs */
class SequenceLogFormatter
{
    private:
        /** @brief Private construtor */
        SequenceLogFormatter() {}

    public:
        /** @brief Format a test done in the sequence
            @note This version format is a comparison sequence test between two numbers to a
                  sequence log
            @param testSuccess Say if the test successed or failed
            @param elementsTested Description of the element tested
            @param measureDone The measure done and tested
            @param infTolerance The inferior tolerance of the test
            @param supTolerance The superior tolerance of the test
            @param testDescription The description of the test
            @return The sequence log formatted */
        static SequenceLogData formatTest(bool testSuccess,
                                          const QString &elementTested,
                                          const Number &measureDone,
                                          const Number &infTolerance,
                                          const Number &supTolerance,
                                          const QString &testDescription = QString());

        /** @brief Format a test done in the sequence
            @note This version format compare a measured logic level with an expected one
            @param testSuccess Say if the test successed or failed
            @param elementsTested Description of the element tested
            @param measuredLogicLevel The logic level measured
            @param expectedLogicLevel The inferior tolerance of the test
            @param testDescription The description of the test
            @return The sequence log formatted */
        static SequenceLogData formatTest(bool testSuccess,
                                          const QString &elementTested,
                                          qint8 measuredLogicLevel,
                                          qint8 expectedLogicLevel,
                                          const QString &testDescription = QString());

        /** @brief Format a test done in the sequence
            @note This version format a simple test
            @param testSuccess Say if the test successed or failed
            @param elementsTested Description of the element tested
            @param testDescription The description of the test
            @return The sequence log formatted */
        static SequenceLogData formatTest(bool testSuccess,
                                          const QString &elementTested,
                                          const QString &testDescription = QString());

        /** @brief Format a log to represent a problem occurred while testing
            @param elementsTested Description of the element tested
            @param problemDescription The problem description
            @param testDescription The description of the test
            @return The sequence log formatted */
        static SequenceLogData formatTestProblem(const QString &elementTested,
                                                 const QString &problemDescription,
                                                 const QString &testDescription = QString());

        /** @brief Format the first log of a sequence log file
            @note This version of the first log only contains the serial number of the board
                  processed
            @param serialNumber The serial number of the board
            @return The log formatted */
        static SequenceLogData formatFirstLog(const QString &serialNumber);

        /** @brief Format a problem linked to the business process (the sequence test logic)
            @param problemDescription The problem description
            @return The log formatted */
        static SequenceLogData formatABusinessProblem(const QString &problemDescription);

        /** @brief Format a problem linked to the application process (the sequence test logic)
            @param problemDescription The problem description
            @return The log formatted */
        static SequenceLogData formatAnApplicationProblem(const QString &problemDescription);

        /** @brief Stringified the sequence log
            @note The log is formatted like this:
                        timestamp [logType] LogCategory - logMsg
                  Example:
                        123456789 [Dbug] BusinessScope - log message
            @param logStruct The sequence log to stringify
            @param formatType The log format to apply
            @return The log stringified */
        static QString toString(const SequenceLogData &logStruct,
                                SequenceLogFormatType::Enum formatType =
                                                                SequenceLogFormatType::DisplayAll);

    private:
        /** @brief Format a generic test done in the sequence
            @param testSuccess Say if the test successed or failed
            @param elementsTested Description of the element tested
            @param testDescription The description of the test
            @param testDetail The detail of the test
            @return The sequence log formatted */
        static SequenceLogData formatPartialTest(bool testSuccess,
                                                 const QString &elementTested,
                                                 const QString &testDescription,
                                                 const QString &testDetail = QString());
};
