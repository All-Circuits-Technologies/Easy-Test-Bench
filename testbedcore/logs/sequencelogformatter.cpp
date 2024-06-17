// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "sequencelogformatter.hpp"

#include <QDateTime>


SequenceLogData SequenceLogFormatter::formatPartialTest(bool testSuccess,
                                             const QString &elementTested,
                                             const QString &testDescription,
                                             const QString &testDetail)
{
    QString descriptionTmp;

    if(!testDescription.isEmpty())
    {
        descriptionTmp = QString("Test description: %1\r\n").arg(testDescription);
    }

    return { testSuccess ? LogMsgType::Info : LogMsgType::Critical,
             SequenceLogCategory::BusinessScope,
             QString("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\r\n"
                     "Element tested: %1\r\n"
                     "%2"
                     "Test success: %3"
                     "%4%5").arg(elementTested,
                                 descriptionTmp,
                                 testSuccess ? "True" : "False",
                                 testDetail.isEmpty() ? "" : "\r\n",
                                 testDetail) };
}

SequenceLogData SequenceLogFormatter::formatTest(bool testSuccess,
                                                 const QString &elementTested,
                                                 const Number &measureDone,
                                                 const Number &infTolerance,
                                                 const Number &supTolerance,
                                                 const QString &testDescription)
{
    return formatPartialTest(testSuccess,
                             elementTested,
                             testDescription,
                             QString("Value measured: %1\r\n"
                                     "Mesure test bounds: %2 <= X <= %3")
                                                             .arg(measureDone.toString(),
                                                                  infTolerance.toString(),
                                                                  supTolerance.toString()));
}

SequenceLogData SequenceLogFormatter::formatTest(bool testSuccess,
                                                 const QString &elementTested,
                                                 qint8 measuredLogicLevel,
                                                 qint8 expectedLogicLevel,
                                                 const QString &testDescription)
{
    return formatPartialTest(testSuccess,
                             elementTested,
                             testDescription,
                             QString("Measured logic level: %1\r\n"
                                     "Expected logic level: %2").arg(measuredLogicLevel)
                                                                .arg(expectedLogicLevel));
}

SequenceLogData SequenceLogFormatter::formatTest(bool testSuccess,
                                                 const QString &elementTested,
                                                 const QString &testDescription)
{
    return formatPartialTest(testSuccess,
                             elementTested,
                             testDescription);
}

SequenceLogData SequenceLogFormatter::formatTestProblem(const QString &elementTested,
                                                        const QString &problemDescription,
                                                        const QString &testDescription)
{
    return formatPartialTest(false,
                             elementTested,
                             testDescription,
                             QString("An error occurred while testing: %1").arg(problemDescription));
}

SequenceLogData SequenceLogFormatter::formatFirstLog(const QString &serialNumber)
{
    return { LogMsgType::Info,
             SequenceLogCategory::BusinessScope,
             QString("Board serial number: %1").arg(serialNumber) };
}

SequenceLogData SequenceLogFormatter::formatABusinessProblem(const QString &problemDescription)
{
    return { LogMsgType::Warning,
             SequenceLogCategory::BusinessScope,
             problemDescription };
}

SequenceLogData SequenceLogFormatter::formatAnApplicationProblem(const QString &problemDescription)
{
    return { LogMsgType::Warning,
             SequenceLogCategory::ApplicationScope,
             problemDescription };
}

QString SequenceLogFormatter::toString(const SequenceLogData &logStruct,
                                       SequenceLogFormatType::Enum formatType)
{
    switch(formatType)
    {
        case SequenceLogFormatType::DisplayAll:
            return QString("%1 %2 %3 - %4")
                    .arg(QDateTime::fromMSecsSinceEpoch(logStruct.getTimestampMSecsSinceEpoch())
                                                                    .toString(Qt::ISODateWithMs),
                         LogMsgType::toLogString(logStruct.getType()),
                         SequenceLogCategory::toString(logStruct.getCategory()),
                         logStruct.getText());

        case SequenceLogFormatType::OnlyDisplayMsg:
            return logStruct.getText();

        case SequenceLogFormatType::Unknown:
            break;
    }

    return logStruct.getText();
}
