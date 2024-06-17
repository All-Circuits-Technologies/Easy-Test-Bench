// SPDX-FileCopyrightText: 2021 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "blocs/abasepagebloc.hpp"

#include "blocs/companions/menucompanion.hpp"
#include "core/hmimanager.hpp"
#include "models/menuitemmodel.hpp"

#include <QQmlEngine>


ABasePageBloc::ABasePageBloc(QObject *parent) :
    AbstractBloc(parent),
    _navigationManager(HmiManager::instance().accessManager<NavigationManager>()),
    _fileMenuModel(new GenericListModel<MenuItemModel>(this))
{
}

ABasePageBloc::~ABasePageBloc()
{
}

void ABasePageBloc::fillFileMenu()
{
    _fileMenuModel->append(MenuCompanion::getMenuItem(MenuItemName::preferences, this));
    _fileMenuModel->append(MenuCompanion::getMenuItem(MenuItemName::exit, this));
}

void ABasePageBloc::onMenuItemClicked(int index)
{
    _fileMenuModel->at(index)->itemClicked();
}

void ABasePageBloc::initBloc()
{
    fillFileMenu();

    emit blocInitialized();
}
