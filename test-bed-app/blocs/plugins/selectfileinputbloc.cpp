// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "selectfileinputbloc.hpp"

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QQmlEngine>

#include "models/plugins/selectfileinputmodel.hpp"


SelectFileInputBloc::SelectFileInputBloc(QObject *parent) :
    APluginInputBloc(parent),
    _folderPath(QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)))
{
    connect(this, &APluginInputBloc::inputModelChanged,
            this, &SelectFileInputBloc::onModelUpdated);
}

void SelectFileInputBloc::onValidateButtonCLicked(const QString &value)
{
    if(!setValue(value))
    {
        emit error();
    }
}

void SelectFileInputBloc::onFileSelected(const QUrl &fileUrl)
{
    manageValidation(fileUrl);
}

bool SelectFileInputBloc::manageValidation(const QUrl &value)
{
    SelectFileInputModel *castedModel = getSelectFileInputModel();
    QString errorTxt = validateFilePath(value, *castedModel);
    bool valueValidate = errorTxt.isEmpty();

    if(valueValidate)
    {
        QFileInfo file(value.toLocalFile());
        castedModel->setValueSet(file.absoluteFilePath());
        setFolderPath(QUrl::fromLocalFile(file.dir().absolutePath()));
    }

    setErrorTxt(errorTxt);
    setValidBtnEnabled(valueValidate);

    return valueValidate;
}

void SelectFileInputBloc::setErrorTxt(const QString &errorTxt)
{
    if(errorTxt != _errorTxt)
    {
        _errorTxt = errorTxt;
        emit errorTxtChanged(errorTxt);
    }
}

void SelectFileInputBloc::setValidBtnEnabled(bool enabled)
{
    if(enabled != _validBtnEnabled)
    {
        _validBtnEnabled = enabled;
        emit validBtnEnabledChanged(enabled);
    }
}

void SelectFileInputBloc::setFolderPath(const QUrl &folderPath)
{
    if(folderPath != _folderPath)
    {
        _folderPath = folderPath;
        emit folderPathUpdated(_folderPath);
    }
}

void SelectFileInputBloc::onModelUpdated()
{
    SelectFileInputModel *castedModel = getSelectFileInputModel();

    QString path = castedModel->getValueSet().toString();

    if(!path.isEmpty())
    {
        // There is an existent value which needs to be tested
        manageValidation(QUrl::fromLocalFile(path));
    }

    emit castedModelChanged(castedModel);
}

QString SelectFileInputBloc::validateFilePath(const QUrl &value,
                                              const SelectFileInputModel &castedModel)
{
    if(!castedModel.isRequired() && value.isEmpty())
    {
        return {""};
    }

    if(!value.isValid())
    {
        return tr(noFileSelectedError);
    }

    QFileInfo file(value.toLocalFile());

    if(!file.exists())
    {
        return tr(fileDoesntExistError);
    }

    if(file.isDir() && !castedModel.isSelectDir())
    {
        return tr(notAFileError);
    }

    if(file.isFile() && castedModel.isSelectDir())
    {
        return tr(notADirError);
    }

    return {""};
}


SelectFileInputModel *SelectFileInputBloc::getSelectFileInputModel() const
{
    return getCastedModel<SelectFileInputModel>();
}

void SelectFileInputBloc::registerQmlTypes()
{
    qmlRegisterType<SelectFileInputBloc>("app.blocs", 1, 0, "SelectFileInputBloc");
}
