// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "loggers/asavinglogfile.hpp"
#include "logsutility/loggingstrategyoption.hpp"

#include "abstractpluginelements/registering/registersequencemoduleutility.hpp"


/** @brief This module creates a file and save all the logs in it.
    @note A new file is created by day. */
class OnlyOneFile : public ASavingLogFile
{
    Q_OBJECT
    REGISTER_SEQUENCE_MODULE(OnlyOneFile)

    public:
        /** @brief Class constructor
            @param parent The class parent */
        Q_INVOKABLE explicit OnlyOneFile(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~OnlyOneFile() override;

    protected:
        /** @brief ASavingLogFile::startLoggingProcess */
        virtual bool startLoggingProcess(const QString &logsFolderPath,
                                         const JsonArray &jsonObjectInputValues) override;

    private:
        inline static const LoggingStrategyOption::Enums _defaultStrategies = {
            LoggingStrategyOption::File_StoreInDayFolder    |
            LoggingStrategyOption::File_StoreInMonthFolder  |
            LoggingStrategyOption::File_StoreInYearFolder
        };
};
