// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "onlyonefile.hpp"

#include "logsutility/saveloginfilesthread.hpp"


OnlyOneFile::OnlyOneFile(QObject *parent) :
    ASavingLogFile(staticMetaObject.className(), parent)
{
}

OnlyOneFile::~OnlyOneFile()
{
}

bool OnlyOneFile::startLoggingProcess(const QString &logsFolderPath,
                                      const JsonArray &jsonObjectInputValues)
{
    Q_UNUSED(jsonObjectInputValues)

    RETURN_IF_FALSE(accessLogsFileThread()->resetToOneFilePerDayStrategy(
        logsFolderPath,
        getLogFileSuffix(),
        _defaultStrategies,
        getMaxFolderSizeInMo()));

    RETURN_IF_FALSE(accessLogsFileThread()->startLogging());

    return true;
}
