// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Useful manager for the QML classes to calculate the real size of each items
    @note We have built the application thanks to a mockup, with a fixed size, if the final device
          has a different size, the widget needs also to have a different size but with keeping the
          initial ratios
    @note Because the screen ratio can be reallly different, and to avoid to have deformed widgets,
          it's advised to use the @ref SizeManager::getSizeBaseOnAppWidth method for the height when
          keeping the widget ratio is important (for instance for an icon).
          When you are using lists, fullHeight widget, etc... you can use the
          @ref SizeManager::getSizeBaseOnAppHeight method */
class SizeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int appHeight READ getAppHeight WRITE setAppHeight NOTIFY appHeightUpdated)
    Q_PROPERTY(int appWidth READ getAppWidth WRITE setAppWidth NOTIFY appWidthUpdated)

    public:
        /** @brief Class constructor
            @param defaultAppHeight The default application height size
            @param defaultAppWidth The default application width size
            @param mockupHeight The mockup height size
            @param mockupWidth The mockup width size
            @param parent The parent instance object */
        explicit SizeManager(int defaultAppHeight,
                             int defaultAppWidth,
                             int mockupHeight,
                             int mockupWidth,
                             QObject *parent = nullptr);

    public:
        /** @brief Get the size based on the Height of the application window
            @param heightInMockup The height used in the mockup
            @return The height to use in the application */
        Q_INVOKABLE int getSizeBasedOnAppHeight(int heightInMockup) const
        { return (heightInMockup * _appHeight) / _mockupHeight; }

        /** @brief Get the size based on the width of the application window
            @param widthInMockup The width used in the mockup
            @return The width to use in the application */
        Q_INVOKABLE int getSizeBasedOnAppWidth(int widthInMockup) const
        { return (widthInMockup * _appWidth) / _mockupWidth; }

        /** @brief Get the application height */
        Q_INVOKABLE int getAppHeight() const { return _appHeight; }

        /** @brief Get the application width */
        Q_INVOKABLE int getAppWidth() const { return _appWidth; }

        /** @brief Set the application height */
        Q_INVOKABLE void setAppHeight(int appHeight);

        /** @brief Set the application width */
        Q_INVOKABLE void setAppWidth(int appWidth);

    signals:
        /** @brief Emitted when the app height has been changed */
        void appHeightUpdated(int height);

        /** @brief Emitted when the app width has been changed */
        void appWidthUpdated(int width);

    public:
        static const constexpr int MockupHeight = 800;
        static const constexpr int MockupWidth = 1280;

    private:
        int _appHeight;
        int _appWidth;
        int _mockupHeight;
        int _mockupWidth;
};
