// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "databaseinfosjsonparser.hpp"

#include <QDebug>
#include <QJsonObject>

#include "database/databaseauthinfo.hpp"
#include "jsonutility/jsonhelper.hpp"
#include "project/projectfile.hpp"
#include "testbedcore/parsers/jsonparsersconstants.hpp"
#include "testbedcore/testbedglobal.hpp"


DatabaseInfosJsonParser::DatabaseInfosJsonParser(const QJsonObject &dbObject,
                                                 ProjectFile &projectFile,
                                                 QObject *parent) :
    AJsonParser(parent),
    _dbObject(dbObject),
    _projectFile(projectFile)
{
}

bool DatabaseInfosJsonParser::loadAndParseDatabaseInfo(const QJsonObject &databaseInfoObject,
                                                       ProjectFile &projectFile)
{
    DatabaseInfosJsonParser dbParser(databaseInfoObject, projectFile);
    NOT_IMPLEMENTED();
    return true;
}

bool DatabaseInfosJsonParser::loadDatabaseInfo()
{
    QString name;
    QString url;
    QString authName;
    
    if(!JsonHelper::getString(_dbObject, JsonParsersConstants::Generic::NameAttribute, name) ||
       !JsonHelper::getString(_dbObject, JsonParsersConstants::Project::UrlName, url) ||
       !JsonHelper::getString(_dbObject,
                              JsonParsersConstants::Project::AuthNameAttribute,
                              authName,
                              true))
    {
        return false;
    }

    DatabaseAuthInfo *dbAuthInfo = new DatabaseAuthInfo(&_projectFile);

    dbAuthInfo->setName(name);
    dbAuthInfo->setUrl(url);
    dbAuthInfo->setAuthName(authName);

    _projectFile.setDatabaseAuthInfo(dbAuthInfo);

    return true;
}
