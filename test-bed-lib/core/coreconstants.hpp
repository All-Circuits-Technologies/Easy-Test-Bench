// SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QStringList>

#include "logsutility/logmsgtype.hpp"


/** @brief Contains the global constants of the library */
namespace CoreConstants
{
#ifdef ACT_BUILD_RELEASE
    /** @brief Say that the library has been built in release mode */
    static const constexpr bool isReleaseMode = true;
#else
    /** @brief Say that the library has been built in debug mode */
    static const constexpr bool isReleaseMode = false;
#endif

    /** @brief Default organization name set to the QCoreApplication if the global variable
               ORGANIZATION_NAME has not been set in the .pro file */
    static const constexpr char *defaultOrganizationName = "ACTechnologies";

    /** @brief Suffix of sequence files */
    static const QStringList sequenceFileSuffixes = { "tbs", "ytbs" };

    namespace Bank
    {
        /** @brief Relative path to access the banks folder (from executable) */
        static const constexpr char *folderRelativePath = "banks";

        /** @brief Key word used to identify that the lib included is in debug */
        static const constexpr char *pluginDebugKey = "debug";
    }

    namespace Logs
    {
#ifdef ACT_BUILD_RELEASE
        /** @brief Default console log criticity, used by the logs application */
        static const constexpr LogMsgType::Enum defaultConsoleCriticity = LogMsgType::Warning;
#else
        /** @brief Default console log criticity, used by the logs application */
        static const constexpr LogMsgType::Enum defaultConsoleCriticity = LogMsgType::Debug;
#endif

        /** @brief Default file saving log criticity, used by the logs application */
        static const constexpr LogMsgType::Enum defaultFileSavingCriticity = LogMsgType::Warning;

        /** @brief Default translate files folder */
        static const constexpr char *defaulfTranslateFilesFolder = "";

        /** @brief Default folder path, where to write the application logs */
        static const constexpr char *defaultFolderPath = "logs";

        /** @brief Default folder path for business logs */
        static const constexpr char *defaultBusinessFolderPath = "businessLogs";

        /** @brief Default log file name pattern */
        static const constexpr char *defaultLogFileNamePattern = "testbed.log";
    }

    namespace Database
    {
        /** @brief Default database folder path */
        static const constexpr char *defaultDatabasePath = "data/data";
    }

    namespace Yaml
    {
        /** @brief Default yp program path */
        static const constexpr char *defaultYqProgramPath = "";

        /** @brief Default yp working directory path */
        static const constexpr char *defaultYamlWorkerDirPath = ".tmp_yaml";
    }
}
