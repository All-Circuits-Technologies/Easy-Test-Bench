// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "navigationmanager.hpp"

#include <QQmlEngine>


NavigationManager::NavigationManager(QObject *parent) : QObject(parent)
{
}

void NavigationManager::setCurrentPage(NavigationPageType::Enum currentPage)
{
    if(currentPage != _currentPage)
    {
        _currentPage = currentPage;
        emit currentPageChanged(currentPage);
    }
}

QString NavigationManager::getPageName(NavigationPageType::Enum page)
{
    return NavigationPageType::toString(page);
}

void NavigationManager::goToLoginPage()
{
    goToPage(NavigationPageType::LoginPage, NavigationBehavior::PopAllAndPush);
}

void NavigationManager::goToProjectSelectionPage(NavigationBehavior::Enum behavior)
{
    goToPage(NavigationPageType::ProjectSelectionPage, behavior);
}

void NavigationManager::goToSequenceSelectionPage()
{
    goToPage(NavigationPageType::SequenceSelectionPage, NavigationBehavior::PushOrPop);
}

void NavigationManager::goToSequenceRunPage()
{
    goToPage(NavigationPageType::SequenceRunPage, NavigationBehavior::PopAllAndPush);
}

void NavigationManager::registerQmlTypes()
{
    NavigationBehavior::registerQmlTypes();
    NavigationPageType::registerQmlTypes();
}

void NavigationManager::registerMetaTypes()
{
    NavigationBehavior::registerMetaTypes();
    NavigationPageType::registerMetaTypes();
}

void NavigationManager::goToPage(NavigationPageType::Enum page, NavigationBehavior::Enum behavior)
{
    if(page != _currentPage)
    {
        emit changePageTo(page, behavior);
    }
}
