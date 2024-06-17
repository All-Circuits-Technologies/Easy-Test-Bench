// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "appapicjsoncheckerservice.hpp"
#include "appapicqstringcopyhelper.hpp"

#include <QJsonDocument>

#include "errors/sequenceloadingerrorhandler.hpp"
#include "jsonparsers/sequencejsonarrayloader.hpp"
#include "sequence/checksequencevalidity.hpp"
#include "sequence/nodessequencemanager.hpp"


AppApiCJsonCheckerService::AppApiCJsonCheckerService()
{
}

bool AppApiCJsonCheckerService::checkJson(char *json, char **error, char **nodeName)
{
    *error = nullptr;
    *nodeName = nullptr;
    QString jsonQString = json;
    QJsonParseError jsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonQString.toUtf8(), &jsonParseError);
    if (doc.isNull())
    {
        *error = AppApiCQStringCopyHelper::copyQString(jsonParseError.errorString());
        return false;
    }
    if(!doc.isArray())
    {
        *error = AppApiCQStringCopyHelper::copyQString(ErrNotAnArray);
        return false;
    }
    QJsonArray jsonArray = doc.array();

    NodesSequenceManager nodesSequenceManager = NodesSequenceManager();
    SequenceLoadingErrorHandler errHandler = SequenceLoadingErrorHandler();

    bool ok = SequenceJsonArrayLoader::loadAndParseSequenceFile(nodesSequenceManager,
                                                                jsonArray,
                                                                &errHandler);
    if (!ok)
    {
        *error = AppApiCQStringCopyHelper::copyQString(ErrParsingSequence);
        return false;
    }
    QSharedPointer<NodesSequence> nodes = nodesSequenceManager.getNodesSequence();

    QString err;
    QSharedPointer<ANode> node;

    if (!CheckSequenceValidity::isValid(nodes, err, node))
    {
        *error = AppApiCQStringCopyHelper::copyQString(err);
        *nodeName = AppApiCQStringCopyHelper::copyQString(node->getInstanceName());
        return false;
    }
    return true;
}
