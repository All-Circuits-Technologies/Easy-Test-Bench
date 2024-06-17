// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include <QObject>


/** @brief Class that manages the content of a menu item */
class MenuItemModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool enabled READ isEnabled NOTIFY enabledChanged)

    /** @brief This property is used to inform others that the menu button has been clicked
        @note Only the write method is used from the QML code */
    Q_PROPERTY(bool itemClicked READ isClicked WRITE itemClicked NOTIFY clicked)

    public:
        /** @brief Class constructor
            @param parent The optional QObject parent */
        explicit MenuItemModel(QObject *parent = nullptr);

    public:
        /** @brief Get the name displayed on the menu item */
        const QString &getName() const { return _name; }

        /** @brief Set the name displayed on the menu item */
        void setName(const QString &name);

        /** @brief Test if the menu item is enabled */
        bool isEnabled() const { return _isEnabled; }

        /** @brief Set the menu item state (if it's enabled or not) */
        void setEnabled(bool enabled);

        /** @brief This getter is needed to have an "itemClicked" property but return always
                   false */
        bool isClicked() const { return false; }

        /** @brief Call to emit a signal when item is clicked */
        void itemClicked(bool hasClicked = true);

    signals:
        /** @brief Emited when function 'itemClicked()' is called */
        void clicked();

        /** @brief Emited when function name changed
            @param name The new name */
        void nameChanged(const QString &name);

        /** @brief Emitted when the enabled value changed
            @param enabled The menu item enabled value*/
        void enabledChanged(bool enabled);

    private:
        QString _name;
        bool _isEnabled{true};
};
