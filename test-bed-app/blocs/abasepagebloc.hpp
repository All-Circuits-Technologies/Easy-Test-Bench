// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "blocs/abstractbloc.hpp"

#include "models/genericlistmodel.hpp"

class MenuItemModel;
class NavigationManager;


/** @brief Backend class linked with pages of the app*/
class ABasePageBloc : public AbstractBloc
{
    Q_OBJECT

    Q_PROPERTY(GenericListModel<MenuItemModel> *fileMenuModel
               READ getFileMenuModel
               NOTIFY blocInitialized)

    public:
        /** @brief Class constructor
            @param parent The optional qObject parent */
        explicit ABasePageBloc(QObject *parent = nullptr);

        /** @brief Class destructor */
        virtual ~ABasePageBloc() override;

    public:
        /** @brief Called when a menu item is clicked
            @param index Index of the item clicked */
        Q_INVOKABLE void onMenuItemClicked(int index);

    protected slots:
        /** @see AbstractBloc::initBloc */
        virtual void initBloc() override;

    protected:
        /** Get a NavigationManager Reference */
        NavigationManager &getNavigationManager() const { return _navigationManager; }

        /** @brief Get file menu model */
        GenericListModel<MenuItemModel> *getFileMenuModel() { return _fileMenuModel; }

        /** @brief Fill the file menu with the proper items */
        virtual void fillFileMenu();

    private:
        NavigationManager &_navigationManager;
        GenericListModel<MenuItemModel> *_fileMenuModel{nullptr};
};
