// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#pragma once

#include "jsonparsers/ajsonparser.hpp"

#include <QDir>
#include <QFile>
#include <QString>

#include <functional>

class TopologyFile;
class BmsGadget;


/** @brief This class loads the content of a topology file.
    It parses its content and create the corresponding set of objects representing it. */
class TopologyFileJsonParser : public AJsonParser
{
    Q_OBJECT

    public:
        /** @brief Class constructor dedicated to on-disk files
            @note This call will not start parsing. You must call @ref load to do that.
            @param topology Topology Qt representation to load. Its FilePath property must be set,
                            it is used to load stage 1 (common data and sub-files pointers)
                            and stage 2 (sub-file logical content) content.
            @param parent Qt parentship */
        explicit TopologyFileJsonParser(TopologyFile &topology,
                                        QObject *parent = nullptr);

    public:
        /** @brief Parses the file, populating topologyFile objects from file content
            @return True upon success,
                    false upon failure (see @ref getError) */
        bool load();

    private:
        /** @brief Load a qobject/qgadget from a file
            @param obj Pre-allocated object to load JSON file into
            @param fileName Relative (to _workDir) or absolute file to load
            @return True upon success, false upon failure */
        bool updateObjFromFileName(BmsGadget &obj, const QString &fileName);

        /** @brief Load a qobject/qgadget from a stream
            @param obj Pre-allocated object to load JSON file into
            @param fileName Relative (to _workDir) or absolute file to load
            @return True upon success, false upon failure */
        bool updateObjFromStream(BmsGadget &obj, QIODevice &io);


    private:
        /** @brief Project base path */
        const QDir _workDir;

        /** @brief Target Qt object representation */
        TopologyFile &_topology;
};
