// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief List of file loading error types **/
class ErrorType : QObject
{
    Q_OBJECT

    public:
        enum Enum
        {
            MissingProjectFile,
            MissingSequenceFile,
            InaccessibleFile,
            UnreadableSequence,
            UnreadableProject,
            UnsupportedFileFormat
        };

    public:
        /** @brief Return a translation key associated to an error type
            @param errorType An enum value related to the error type
            @return A translation key **/
        static const char *getTrKey(ErrorType::Enum errorType);

    private:
        static const constexpr char *UnknownErrorKey = QT_TR_NOOP("(TR) Unknown error type");
        static const constexpr char *MissingProjectFileKey = QT_TR_NOOP("(TR) Project file is "
                                                                        "missing at the location: "
                                                                        "%1, would you like to "
                                                                        "remove it from recent "
                                                                        "projects?");
        static const constexpr char *MissingSequenceFileKey = QT_TR_NOOP("(TR) Sequence file is "
                                                                         "missing at the location: "
                                                                         "%1, would you like to "
                                                                         "remove it from recent "
                                                                         "projects?");
        static const constexpr char *InaccessibleFileKey = QT_TR_NOOP("(TR) The file can't be "
                                                                      "read, it is not in JSON "
                                                                      "format or it is "
                                                                      "inaccessible");
        static const constexpr char *UnreadableSequenceKey = QT_TR_NOOP("(TR) The sequence file "
                                                                        "contains one or more "
                                                                        "errors");
        static const constexpr char *UnreadableProjectKey = QT_TR_NOOP("(TR) The project file "
                                                                       "contains one or more "
                                                                       "errors");
        static const constexpr char *UnsupportedFileFormatKey = QT_TR_NOOP("(TR) The file format: "
                                                                           "%1, is not supported "
                                                                           "by the library");
};
