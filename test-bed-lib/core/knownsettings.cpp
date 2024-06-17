// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
// SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "knownsettings.hpp"

#include <QDir>
#include <QMetaEnum>

#include "core/coreconstants.hpp"

const KnownSettingDetails KnownSettings::_defaultSettingDetail = {};

QHash<KnownSettings::Enum, KnownSettingDetails> KnownSettings::_knownSettingsDetails = {};


KnownSettings::Enum KnownSettings::parseFromString(const QString &strKnownSetting)
{
    QString lowerValue = strKnownSetting.toLower();

    QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        QString strValue(metaEnum.key(idx));

        if(strValue.toLower() == lowerValue)
        {
            return static_cast<Enum>(metaEnum.value(idx));
        }
    }

    return UnknownSetting;
}

QString KnownSettings::toString(KnownSettings::Enum value)
{
    return QString::fromLatin1(QMetaEnum::fromType<Enum>().valueToKey(value));
}

QVector<KnownSettings::Enum> KnownSettings::getAvailableElements()
{
    QVector<Enum> enumList;
    const QMetaEnum metaEnum = QMetaEnum::fromType<Enum>();

    for(int idx = 0; idx < metaEnum.keyCount(); idx++)
    {
        enumList.append(static_cast<Enum>(metaEnum.value(idx)));
    }

    return enumList;
}

const KnownSettingDetails &KnownSettings::getSettingDetails(KnownSettings::Enum value)
{
    if(Q_UNLIKELY(_knownSettingsDetails.isEmpty()))
    {
        // First initialisation
        _knownSettingsDetails = KnownSettings::initKnownSettingsDetails();
    }

    if(!_knownSettingsDetails.contains(value))
    {
        return _defaultSettingDetail;
    }

    // This is compulsory if we want to return a const/ref, because only a non-const element has
    // the operator: T & operator[].
    return _knownSettingsDetails[value];
}

QVariant KnownSettings::castStringValueToVariant(KnownSettings::Enum knownSetting,
                                                 const QString &strValue)
{
    switch(knownSetting)
    {
        // Cast QString to QVariant
        case PluginPath:
        case ProjectFileDefaultPath:
        case LogAppFolderPath:
        case LogBusinessFolderPath:
        case LogFileNamePattern:
        case TranslateFilesFolderPath:
        case DatabaseFilePath:
        case YqProgramPath:
        case YamlWorkerDirPath:
            return strValue;

        case LogConsoleCriticity:
        case LogFileSavingCriticity:
            return LogMsgType::parseCriticityFromString(strValue);
        // Return null QVariant
        case UnknownSetting:
            return {};
    }

    return {};
}

QHash<KnownSettings::Enum, KnownSettingDetails> KnownSettings::initKnownSettingsDetails()
{
    QHash<KnownSettings::Enum, KnownSettingDetails> knownSettingsDetails;
    QVector<KnownSettings::Enum> knownSettings = getAvailableElements();
    for(auto citer = knownSettings.cbegin(); citer != knownSettings.cend(); ++citer)
    {
        if(*citer == UnknownSetting)
        {
            knownSettingsDetails.insert(UnknownSetting, _defaultSettingDetail);
            continue;
        }

        QString optionLetterName;
        QString optionDescription;
        QVariant constantDefaultValue;

        switch(*citer)
        {
            case PluginPath:
                optionLetterName = "p";
                optionDescription = "The folder path where to find the build plugins libraries";
                constantDefaultValue = QString(CoreConstants::Bank::folderRelativePath);
                break;

            case ProjectFileDefaultPath:
                optionLetterName = "d";
                optionDescription = "The default folder path to find the project files";
                constantDefaultValue = QDir::homePath();
                break;

            case LogAppFolderPath:
                optionLetterName = "l";
                optionDescription = "The default folder to save the application logs in";
                constantDefaultValue = QString(CoreConstants::Logs::defaultFolderPath);
                break;

            case LogBusinessFolderPath:
                optionLetterName = "b";
                optionDescription = "The default folder to save the business logs in";
                constantDefaultValue = QString(CoreConstants::Logs::defaultBusinessFolderPath);
                break;

            case LogFileNamePattern:
                optionLetterName = "f";
                optionDescription = "The default file name pattern for logs file";
                constantDefaultValue = QString(CoreConstants::Logs::defaultLogFileNamePattern);
                break;

            case LogConsoleCriticity:
                optionLetterName = "c";
                optionDescription = "The default log console criticity";
                constantDefaultValue = CoreConstants::Logs::defaultConsoleCriticity;
                break;

            case LogFileSavingCriticity:
                optionLetterName = "s";
                optionDescription = "The default log file saving criticity";
                constantDefaultValue = CoreConstants::Logs::defaultFileSavingCriticity;
                break;

            case TranslateFilesFolderPath:
                optionLetterName = "t";
                optionDescription = "The default folder of translate files";
                constantDefaultValue = CoreConstants::Logs::defaulfTranslateFilesFolder;
                break;

            case DatabaseFilePath:
                optionLetterName = "q";
                optionDescription = "The default path of the database file";
                constantDefaultValue = CoreConstants::Database::defaultDatabasePath;
                break;

            case YqProgramPath:
                optionLetterName = "y";
                optionDescription = "The path to access yq program";
                constantDefaultValue = CoreConstants::Yaml::defaultYqProgramPath;
                break;

            case YamlWorkerDirPath:
                optionLetterName = "a";
                optionDescription = "The default path of the yaml worker dir path";
                constantDefaultValue = CoreConstants::Yaml::defaultYamlWorkerDirPath;
                break;

            case UnknownSetting:
                // This case can't happen but to avoid the switch warning I add it
                break;
        }

        QString name = toString(*citer).toLower();

        knownSettingsDetails.insert(
            *citer,
            { name, optionLetterName, optionDescription, "<" + name + ">", constantDefaultValue});
    }

    return knownSettingsDetails;
}
