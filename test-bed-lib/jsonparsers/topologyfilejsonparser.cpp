// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfilejsonparser.hpp"

#include <QDebug>
#include <QFileInfo>
#include <QJsonObject>

#include "jsonparsers/bmsgadgetjsonloader.hpp"
#include "topology/topologyfile.hpp"


TopologyFileJsonParser::TopologyFileJsonParser(TopologyFile &topology,
                                               QObject *parent) :
    AJsonParser(parent),
    _workDir(QFileInfo(topology.getFilePath()).absoluteDir()),
    _topology(topology)
{ }

bool TopologyFileJsonParser::load()
{
    bool success = true;

    clearError();

    /* Load topology head (stage 1: name, version and sub-file pathes) */
    if(!updateObjFromFileName(_topology, _topology.getFilePath()))
    {
        AJSONPARSER_LOG_ERR("Failed to load main topology content");
        success = false;
    }

    /* Load a TopologyFile (stage 2: content of pointed sub-files) */
    for(const QString &libraryFilePath : _topology.getLibrariesFilesPath())
    {
        _topology.accessLibraries().append(TopologyFileLibrary());

        if(!updateObjFromFileName(_topology.accessLibraries().last(),
                                  libraryFilePath))
        {
            AJSONPARSER_LOG_ERR("Failed to parse topology library file " + libraryFilePath);
            success &= false;
        }
    }
    for(const QString &instancesFilePath : _topology.getInstancesFilesPath())
    {
        _topology.accessInstances().append(TopologyFileInstances());

        if(!updateObjFromFileName(_topology.accessInstances().last(),
                                  instancesFilePath))
        {
            AJSONPARSER_LOG_ERR("Failed to parse topology instances file " + instancesFilePath);
            success &= false;
        }
    }
    for(const QString &bindingsFilePath : _topology.getBindingsFilesPath())
    {
        _topology.accessBindings().append(TopologyFileBindings());

        if(!updateObjFromFileName(_topology.accessBindings().last(),
                                  bindingsFilePath))
        {
            AJSONPARSER_LOG_ERR("Failed to parse topology bindings file " + bindingsFilePath);
            success &= false;
        }
    }
    for(const QString &routesFilePath : _topology.getRoutesFilesPath())
    {
        _topology.accessRoutes().append(TopologyFileRoutes());

        if(!updateObjFromFileName(_topology.accessRoutes().last(),
                                  routesFilePath))
        {
            AJSONPARSER_LOG_ERR("Failed to parse topology routes file " + routesFilePath);
            success &= false;
        }
    }

    return success;
}

bool TopologyFileJsonParser::updateObjFromFileName(BmsGadget &obj,
                                                   const QString &fileName)
{
    bool success = false;
    QFile fileIo(_workDir.absoluteFilePath(fileName));

    if(!fileIo.exists())
    {
        AJSONPARSER_LOG_ERR("Failed to locate project sub-file " + fileName);
    }
    else if(!fileIo.open(QIODevice::ReadOnly))
    {
        AJSONPARSER_LOG_ERR("Failed to open project sub-file " + fileName);
    }
    else if(!updateObjFromStream(obj, fileIo))
    {
        AJSONPARSER_LOG_ERR("Failed to parse project sub-file " + fileName);
    }
    else
    {
        success = true;
    }

    fileIo.close();
    return success;
}

bool TopologyFileJsonParser::updateObjFromStream(BmsGadget &obj, QIODevice &io)
{
    bool success = false;

    QJsonObject jsonObject;
    BmsGadgetJsonLoader loader;

    if(!loadJsonFileAndGetObject(io, jsonObject))
    {
        /* Load io stream as a JSON object */
        AJSONPARSER_LOG_ERR("Failed to load JSON object");
    }
    else if(!loader.updateObject(jsonObject, obj))
    {
        /* Update Qt object from JSON object */
        AJSONPARSER_LOG_ERR("Failed to load JSON object content");
    }
    else
    {
        success = true;
    }

    return success;
}
