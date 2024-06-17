// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "errortype.hpp"

#include <QDebug>


const char *ErrorType::getTrKey(ErrorType::Enum errorType)
{
    switch(errorType)
    {
        case MissingProjectFile :
            return MissingProjectFileKey;

        case MissingSequenceFile :
            return MissingSequenceFileKey;

        case InaccessibleFile :
            return InaccessibleFileKey;

        case UnreadableSequence :
            return UnreadableSequenceKey;

        case UnreadableProject :
            return UnreadableProjectKey;

        case UnsupportedFileFormat :
            return UnsupportedFileFormatKey;
    }

    qWarning() << "This kind of error doesn't exist or it's not managed";
    return UnknownErrorKey;
}
