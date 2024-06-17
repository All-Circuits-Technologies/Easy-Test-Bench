// SPDX-FileCopyrightText: 2021 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "errors/helpers/fileloadingerrorhelper.hpp"


/** @brief Class providing static methods to handle errors at the lowest level **/
class FileLoadingErrorTypeHelper : public FileLoadingErrorHelper
{
    public:
        /** @brief Used to add an error when the sequence file cannot be found
            @param filePath The path of the file
            @param errorHandler An Errorhandler pointer */
        static void addMissingProjectFileError(const QString &filePath,
                                               FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the project file cannot be found
            @param filePath The path of the file
            @param errorHandler An Errorhandler pointer */
        static void addMissingSequenceFileError(const QString &filePath,
                                                FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the sequence file contains errors
            @param errorHandler An Errorhandler pointer */
        static void addUnreadableSequenceError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the error file contains errors
            @param errorHandler An Errorhandler pointer */
        static void addUnreadableProjectError(FileLoadingErrorHandler *errorHandler);

        /** @brief Used to add an error when the file cannot be accessed
            @param errorHandler An Errorhandler pointer */
        static void addInaccessibleFileError(FileLoadingErrorHandler *errorHandler);

        /** @brief Add unsupported file format error
            @param fileFormat The format of the file not supported
            @param errorHandler An Errorhandler pointer */
        static void addUnsupportedFileFormatError(const QString &fileFormat,
                                                  FileLoadingErrorHandler *errorHandler);
};
