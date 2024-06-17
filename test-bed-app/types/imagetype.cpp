// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "imagetype.hpp"

#include <QFileInfo>
#include <QQmlEngine>

const QVector<QString> ImageType::AnimatedImageExtensions = {"gif"};


ImageType::Enum ImageType::guessFromPath(const QString &imagePath)
{
    if(imagePath.isEmpty())
    {
        return None;
    }

    QFileInfo path(imagePath);
    QString suffix = path.suffix().toLower();

    if(AnimatedImageExtensions.contains(suffix))
    {
        return Animated;
    }

    return Fix;
}

void ImageType::registerMetaTypes()
{
    qRegisterMetaType<ImageType::Enum>("ImageType::Enum");
}

void ImageType::registerQmlTypes()
{
    qmlRegisterType<ImageType>("app.types", 1, 0, "ImageType");
}
