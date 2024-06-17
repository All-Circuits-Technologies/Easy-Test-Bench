// SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "tools/qt/bmsgadget.hpp"

#include "testbedcore/testbedglobal.hpp"
#include "topology/topologyfilelibrary.hpp"
#include "topology/topologyfileinstances.hpp"
#include "topology/topologyfilebindings.hpp"
#include "topology/topologyfileroutes.hpp"

#include <QString>
#include <QStringList>
#include <QVector>


/** @brief Represents the topology file.

    @note A Topology file contains pointer to other files (ie lists of file names),
          save/loaded in *Path properties.
          This TopologyFile object also contains objects loaded from pointed files
          which are loaded in a second pass from pointed files. */
class TopologyFile : public BmsGadget
{
    Q_GADGET
    /* Topology file location */
    SIMPLE_PROPERTY(QString, FilePath)  /**< @brief Location of this file */
    /* Real file content */
    SIMPLE_PROPERTY(QString, Name)  /**< @brief Name given to its topology */
    SIMPLE_PROPERTY(int, Revision)  /**< @brief Overall topolgy revision count */
    SIMPLE_PROPERTY(QVector<QString>, LibrariesFilesPath)  /**< @brief List of library files */
    SIMPLE_PROPERTY(QVector<QString>, InstancesFilesPath)  /**< @brief List of instances files */
    SIMPLE_PROPERTY(QVector<QString>, BindingsFilesPath)   /**< @brief List of bindings files */
    SIMPLE_PROPERTY(QVector<QString>, RoutesFilesPath)     /**< @brief List of routes files */
    /* Logical file content */
    SIMPLE_PROPERTY(QVector<TopologyFileLibrary>,   Libraries)  /**< @brief List of libraries */
    SIMPLE_PROPERTY(QVector<TopologyFileInstances>, Instances)  /**< @brief List of instances */
    SIMPLE_PROPERTY(QVector<TopologyFileBindings>,  Bindings)   /**< @brief List of bindings */
    SIMPLE_PROPERTY(QVector<TopologyFileRoutes>,    Routes)     /**< @brief List of routes */

    public:
        /** @brief Empty invokable constructor useful for QMetaType */
        Q_INVOKABLE explicit TopologyFile();

        /** @brief Topology file path constructor
            @note This constructor is mostly useful to keep a topology file under hand
                  without loading it yet, still being able to do it later using
                  BmsGadgetJsonLoader or TopologyFileJsonParser.
            @param filePath         Full path to current topology file */
        TopologyFile(const QString &filePath);

        /** @brief Real-content options constructor
            @param name             Name given to its topology
            @param revision         Overall topolgy revision count
            @param librariesFilesPath   List of library files
            @param instancesFilesPath   List of instances files
            @param bindingsFilesPath    List of bindings files
            @param routesFilesPath      List of routes files
            @param libraries        List of libraries
            @param instances        List of instances
            @param bindings         List of bindings
            @param routes           List of routes */
        TopologyFile(const QString &name,
                     const int &revision,
                     const QVector<QString> &librariesFilesPath,
                     const QVector<QString> &instancesFilesPath,
                     const QVector<QString> &bindingsFilesPath,
                     const QVector<QString> &routesFilesPath);

        /** @brief Logical-content options constructor
            @param name             Name given to its topology
            @param revision         Overall topolgy revision count
            @param librariesFilesPath   List of library files
            @param instancesFilesPath   List of instances files
            @param bindingsFilesPath    List of bindings files
            @param routesFilesPath      List of routes files
            @param libraries        List of libraries
            @param instances        List of instances
            @param bindings         List of bindings
            @param routes           List of routes */
        TopologyFile(const QString &name,
                     const int &revision,
                     const QVector<TopologyFileLibrary>   &libraries,
                     const QVector<TopologyFileInstances> &instances,
                     const QVector<TopologyFileBindings>  &bindings,
                     const QVector<TopologyFileRoutes>    &routes);

        /** @brief Full-options constructor
            @param filePath         Full path to current topology file
            @param name             Name given to its topology
            @param revision         Overall topolgy revision count
            @param librariesFilesPath   List of library files
            @param instancesFilesPath   List of instances files
            @param bindingsFilesPath    List of bindings files
            @param routesFilesPath      List of routes files
            @param libraries        List of libraries
            @param instances        List of instances
            @param bindings         List of bindings
            @param routes           List of routes */
        TopologyFile(const QString &filePath,
                     const QString &name,
                     const int &revision,
                     const QVector<QString> &librariesFilesPath,
                     const QVector<QString> &instancesFilesPath,
                     const QVector<QString> &bindingsFilesPath,
                     const QVector<QString> &routesFilesPath,
                     const QVector<TopologyFileLibrary>   &libraries,
                     const QVector<TopologyFileInstances> &instances,
                     const QVector<TopologyFileBindings>  &bindings,
                     const QVector<TopologyFileRoutes>    &routes);

        /** @brief Registers current object as well as properties metatypes */
        static void RegisterMetaTypes();

        /** @brief Get current-instance meta object */
        virtual const QMetaObject *metaObject() const override;
};

Q_DECLARE_METATYPE(TopologyFile)
