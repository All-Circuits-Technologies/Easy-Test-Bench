// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Represent the type of an image */
class ImageType : public QObject
{
    Q_OBJECT

    public:
        /** @brief The image type useful to know in order to display it in view */
        enum Enum
        {
            Animated,
            Fix,
            None,
        };
        Q_ENUM(Enum)

    public:
        /** @brief Guess the image type from its path
            @note The image path can be empty (the method will return None)
            @param imagePath The path of the image to guess its type
            @return The type of the image
         */
        static Enum guessFromPath(const QString &imagePath);

        /** @brief Register meta type linked to this enum */
        static void registerMetaTypes();

        /** @brief Register QML type linked to this enum  */
        static void registerQmlTypes();

    private:
        /** @brief The representative extensions of an animated image */
        static const QVector<QString> AnimatedImageExtensions;
};

