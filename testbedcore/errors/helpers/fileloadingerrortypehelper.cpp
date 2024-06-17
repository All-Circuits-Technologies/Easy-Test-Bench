// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "errors/helpers/fileloadingerrortypehelper.hpp"

#include "definesutility/definesutility.hpp"
#include "errors/errortype.hpp"


void FileLoadingErrorTypeHelper::addMissingProjectFileError(const QString &filePath,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    errorHandler->setMissingProjectFileError(true);

    addErrorDescriptor(ErrorLayer::ErrorType,
                       ErrorType::MissingProjectFile,
                       *errorHandler,
                       { filePath });
}

void FileLoadingErrorTypeHelper::addMissingSequenceFileError(const QString &filePath,
                                                             FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    QVector<QVariant> parameters;
    parameters.append(filePath);
    addErrorDescriptor(ErrorLayer::ErrorType,
                       ErrorType::MissingSequenceFile,
                       *errorHandler,
                       parameters);
}

void FileLoadingErrorTypeHelper::addUnreadableSequenceError(FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorType,
                       ErrorType::UnreadableSequence,
                       *errorHandler);
}

void FileLoadingErrorTypeHelper::addUnreadableProjectError(FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    QVector<QVariant> parameters;
    addErrorDescriptor(ErrorLayer::ErrorType,
                       ErrorType::UnreadableProject,
                       *errorHandler,
                       parameters);

}

void FileLoadingErrorTypeHelper::addInaccessibleFileError(FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);

    addErrorDescriptor(ErrorLayer::ErrorType,
                       ErrorType::InaccessibleFile,
                       *errorHandler);
}

void FileLoadingErrorTypeHelper::addUnsupportedFileFormatError(
                                                            const QString &fileFormat,
                                                            FileLoadingErrorHandler *errorHandler)
{
    RETURN_VOID_IF_NULL(errorHandler);
    addErrorDescriptor(ErrorLayer::ErrorType,
                       ErrorType::UnsupportedFileFormat,
                       *errorHandler,
                       { fileFormat });
}
