// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "selectfileinputmodel.hpp"

#include "collectionutility/qvectorhelper.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"

#include <QQmlEngine>


SelectFileInputModel::SelectFileInputModel(QObject *parent) :
    APluginInputModel(parent)
{
}

SelectFileInputModel::SelectFileInputModel(const QSharedPointer<DisplayUiToken> &uiToken,
                                           QObject *parent):
    APluginInputModel(PluginWidgetType::SelectFile,
                      TypeManaged::String,
                      uiToken,
                      parent)
{
    connect(this, &APluginInputModel::valueSetChanged,
            this, &SelectFileInputModel::onValueUpdated);
}

SelectFileInputModel::~SelectFileInputModel()
{
}

QString SelectFileInputModel::getSelectedPath() const
{
    return getValueSet().toString();
}

bool SelectFileInputModel::parseAndSetAttributes(const JsonObject &elemDescription, bool update)
{
    RETURN_IF_FALSE(APluginInputModel::parseAndSetAttributes(elemDescription, update));

    bool selectDir = _selectDir;
    RETURN_IF_FALSE(JsonHelper::getBoolean(elemDescription,
                                           JsonParsersConstants::Hmi::SelectDirAttribute,
                                           selectDir,
                                           true));
    setSelectDir(selectDir);


    if(elemDescription.contains(JsonParsersConstants::Hmi::FilesExtensionsAttribute))
    {
        QVector<QString> filesExtensions;
        RETURN_IF_FALSE(JsonHelper::getStringVector(
                                                elemDescription,
                                                JsonParsersConstants::Hmi::FilesExtensionsAttribute,
                                                filesExtensions));

        QRegExp regExp(fileExtensionsRegExp);
        QVector<QString>::iterator iter = filesExtensions.end();
        while(iter != filesExtensions.begin())
        {
            --iter;

            if(!regExp.exactMatch(*iter))
            {
                qWarning() << "The element: " << *iter << ", is not a rightful file extension";
                iter = filesExtensions.erase(iter);
            }
        }

        setFilesExtensions(filesExtensions);
    }

    QString currentDir = _currentDirectory;
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Hmi::CurrentDirectoryAttribute,
                                          currentDir,
                                          true));
    setCurrentDirectory(currentDir);


    QString defaultValue = getDefaultValue().toString();
    RETURN_IF_FALSE(JsonHelper::getString(elemDescription,
                                          JsonParsersConstants::Hmi::DefaultValueAttribute,
                                          defaultValue,
                                          true));
    setDefaultValue(defaultValue);

    return true;
}

void SelectFileInputModel::onValueUpdated(const QVariant &value)
{
    emit selectedPathChanged(value.toString());
}

void SelectFileInputModel::setSelectDir(bool selectDir)
{
    if(_selectDir != selectDir)
    {
        _selectDir = selectDir;
        emit selectDirChanged(_selectDir);
    }
}

void SelectFileInputModel::setFilesExtensions(const QVector<QString> &filesExtensions)
{
    if(!QVectorHelper::compareList(_filesExtensions, filesExtensions, false))
    {
        _filesExtensions = filesExtensions;
        emit filesExtensionsChanged();
    }
}

void SelectFileInputModel::setCurrentDirectory(const QString &currentDirectory)
{
    if(_currentDirectory != currentDirectory)
    {
        _currentDirectory = currentDirectory;
        emit currentDirectoryChanged(_currentDirectory);
    }
}

void SelectFileInputModel::registerQmlType()
{
    qmlRegisterType<SelectFileInputModel>("app.models", 1, 0, "SelectFileInputModel");
}
