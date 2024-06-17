// SPDX-FileCopyrightText: 2021 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "models/modelsregistrator.hpp"

#include "models/filelistitemmodel.hpp"
#include "models/formtextfieldmodel.hpp"
#include "models/genericlistmodel.hpp"
#include "models/logviewitemmodel.hpp"
#include "models/menuitemmodel.hpp"
#include "models/plugins/aplugininputmodel.hpp"
#include "models/plugins/buttoninputmodel.hpp"
#include "models/plugins/radiolistbuttonsinputmodel.hpp"
#include "models/plugins/selectfileinputmodel.hpp"
#include "models/plugins/stringinputmodel.hpp"
#include "models/scrollablepopupmodel.hpp"


void ModelsRegistrator::registerMetaTypes()
{
    APluginInputModel::registerMetaType();

    GenericListModel<FileListItemModel>::registerMetaTypes("GenericListModel<FileListItemModel>*");
    GenericListModel<MenuItemModel>::registerMetaTypes("GenericListModel<MenuItemModel>*");
    GenericListModel<LogViewItemModel>::registerMetaTypes("GenericListModel<LogViewItemModel>*");
    GenericListModel<APluginInputModel>::registerMetaTypes("GenericListModel<APluginInputModel>*");
    GenericListModel<ButtonInputModel>::registerMetaTypes("GenericListModel<ButtonInputModel>*");
}

void ModelsRegistrator::registerQmlTypes()
{
    APluginInputModel::registerBaseQmlType();
    ButtonInputModel::registerQmlType();
    FormTextFieldModel::registerBaseQmlType();
    LogViewItemModel::registerQmlType();
    RadioListButtonsInputModel::registerQmlType();
    SelectFileInputModel::registerQmlType();
    ScrollablePopUpModel::registerQmlType();
    StringInputModel::registerQmlType();
}
