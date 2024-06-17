// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "settingsmanager.hpp"

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QReadWriteLock>
#include <QSettings>

#include "core/coreconstants.hpp"


SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent),
    _readWriteLock(new QReadWriteLock())
{
}

SettingsManager::~SettingsManager()
{
    if(_settingsFile)
    {
        _settingsFile->sync();
        delete _settingsFile;
    }
}

bool SettingsManager::load()
{
    if(_settingsFile)
    {
        qDebug() << "Process options already called";
        return true;
    }

    QString appPath = QCoreApplication::applicationFilePath().remove(".exe");
    _settingsFile = new QSettings(appPath.append(".ini"), QSettings::IniFormat);

    // Get values from command line options
    QHash<KnownSettings::Enum, QVariant> commandLineValues;
    if(!parseCommandLineOptionsValues(commandLineValues))
    {
        qWarning() << "Can't parse the command lines, continue with other sources";
    }

    qDebug() << "Use Ini file stored here: " << _settingsFile->fileName();

    QVector<KnownSettings::Enum> knownSettings = KnownSettings::getAvailableElements();
    for(auto citer = knownSettings.cbegin(); citer != knownSettings.cend(); ++citer)
    {
        if(*citer == KnownSettings::UnknownSetting)
        {
            // Don't proceed unknown
            continue;
        }

        KnownSettingDetails details = KnownSettings::getSettingDetails(*citer);

        if(details.name.isNull())
        {
            qWarning() << "The known setting has no details: "
                       << KnownSettings::toString(*citer) << ", set value to null";
            _cache[*citer] = QVariant();
            continue;
        }

        QVariant value = commandLineValues.value(*citer, QVariant());

        if(value.isNull())
        {
            // If no command line value given, get ini value
            value = _settingsFile->value(details.name);
        }

        if(value.isNull())
        {
            // If no ini value found, get default value
            value = details.constantDefaultValue;
        }

        if(value.isNull())
        {
            qWarning() << "The setting: " << details.name << ", value hasn't been found "
                       << "anywhere, set value to null";
            continue;
        }

        _cache[*citer] = value;
    }

    return true;
}

void SettingsManager::clear()
{
    _settingsFile->sync();
    delete _settingsFile;
    _settingsFile = nullptr;

    _cache.clear();
}

bool SettingsManager::read(KnownSettings::Enum setting, QVariant &value) const
{
    value.clear();

    if(_settingsFile == nullptr)
    {
        qWarning() << "Can't read the setting value wanted: " << KnownSettings::toString(setting)
                   << ", the settings haven't been loaded";
        return false;
    }

    if(!_cache.contains(setting))
    {
        qWarning() << "Can't read the setting value wanted: " << KnownSettings::toString(setting)
                   << ", it doesn't exist";
        return false;
    }

    _readWriteLock->lockForRead();

    value = _cache[setting];

    _readWriteLock->unlock();

    return true;
}

bool SettingsManager::read(KnownSettings::Enum setting, QString &value) const
{
    QVariant varValue;

    RETURN_IF_FALSE(read(setting, varValue));

    if(!varValue.canConvert<QString>())
    {
        qWarning() << "Can't read the setting value wanted: " << KnownSettings::toString(setting)
                   << ", can't be parsed to string";
        value = QString();
        return false;
    }

    value = varValue.toString();

    return true;
}

bool SettingsManager::write(KnownSettings::Enum setting, const QVariant &value, bool writeToIniFile)
{
    if(_settingsFile == nullptr)
    {
        qWarning() << "Can't read the setting value wanted: " << KnownSettings::toString(setting)
                   << ", the settings haven't been loaded";
        return false;
    }

    _readWriteLock->lockForWrite();

    _cache[setting] = value;

    if(writeToIniFile)
    {
        _settingsFile->setValue(KnownSettings::toString(setting).toLower(), value);
    }

    _readWriteLock->unlock();

    return true;
}

bool SettingsManager::parseCommandLineOptionsValues(
                                            QHash<KnownSettings::Enum, QVariant> &commandLineValues)
{
    QCommandLineParser parser;

    parser.addHelpOption();
    parser.addVersionOption();

    QVector<KnownSettings::Enum> elementsWithOption = KnownSettings::getAvailableElements();

    QVector<KnownSettings::Enum>::iterator iter = elementsWithOption.begin();

    while(iter != elementsWithOption.end())
    {
        KnownSettingDetails details = KnownSettings::getSettingDetails(*iter);

        if(details.name.isNull())
        {
            // The option info hasn't been defined, so remove it from the list to parse
            iter = elementsWithOption.erase(iter);
            continue;
        }

        QStringList names = { details.name };

        if(!details.optionLetterName.isNull())
        {
            // Add the option letter if it has been defined
            names.prepend(details.optionLetterName);
        }

        if(!parser.addOption({ names, details.optionDescription, details.optionValueName }))
        {
            qWarning() << "A problem occurred when tryied to add the setting option to parse: "
                       << details.name << " (the cause can be: the name is empty or it "
                       << "already exists; check the option letter)";
            return false;
        }

        iter++;
    }

    if(!parser.parse(QCoreApplication::arguments()))
    {
        // We use parse and not process, because we don't want to do an exit if an error occurs
        qWarning() << "A problem occurred when parsing the command line arguments: "
                   << parser.errorText();
        return false;
    }

    for(KnownSettings::Enum knownSetting : elementsWithOption)
    {
        KnownSettingDetails details = KnownSettings::getSettingDetails(knownSetting);

        if(!parser.isSet(details.name))
        {
            // The option hasn't been set
            continue;
        }

        QString value = parser.value(details.name);
        commandLineValues[knownSetting] = KnownSettings::castStringValueToVariant(knownSetting,
                                                                                  value);
    }

    return true;
}
