// SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
// SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include "topologyfile.hpp"

#include "collectionutility/qvariantlisthelper.hpp"


TopologyFile::TopologyFile() :
    TopologyFile({}, {}, 0, {}, {}, {}, {}, {}, {}, {}, {})
{ }

TopologyFile::TopologyFile(const QString &filePath) :
    TopologyFile(filePath, {}, 0, {}, {}, {}, {}, {}, {}, {}, {})
{ }

TopologyFile::TopologyFile(const QString &name,
                           const int &revision,
                           const QVector<QString> &librariesFilesPath,
                           const QVector<QString> &instancesFilesPath,
                           const QVector<QString> &bindingsFilesPath,
                           const QVector<QString> &routesFilesPath) :
    TopologyFile({},
                 name, revision,
                 librariesFilesPath, instancesFilesPath, bindingsFilesPath, routesFilesPath,
                 {}, {}, {}, {})
{ }

TopologyFile::TopologyFile(const QString &name,
                           const int &revision,
                           const QVector<TopologyFileLibrary> &libraries,
                           const QVector<TopologyFileInstances> &instances,
                           const QVector<TopologyFileBindings> &bindings,
                           const QVector<TopologyFileRoutes> &routes) :
    TopologyFile({},
                 name, revision,
                 {}, {}, {}, {},
                 libraries, instances, bindings, routes)
{ }

TopologyFile::TopologyFile(const QString &filePath,
                           const QString &name,
                           const int &revision,
                           const QVector<QString> &librariesFilesPath,
                           const QVector<QString> &instancesFilesPath,
                           const QVector<QString> &bindingsFilesPath,
                           const QVector<QString> &routesFilesPath,
                           const QVector<TopologyFileLibrary> &libraries,
                           const QVector<TopologyFileInstances> &instances,
                           const QVector<TopologyFileBindings> &bindings,
                           const QVector<TopologyFileRoutes> &routes) :
    BmsGadget(),
    _FilePath(filePath),
    _Name(name),
    _Revision(revision),
    _LibrariesFilesPath(librariesFilesPath),
    _InstancesFilesPath(instancesFilesPath),
    _BindingsFilesPath(bindingsFilesPath),
    _RoutesFilesPath(routesFilesPath),
    _Libraries(libraries),
    _Instances(instances),
    _Bindings(bindings),
    _Routes(routes)
{ }

void TopologyFile::RegisterMetaTypes()
{
    QMetaType::registerComparators<QVector<QString>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 QString);

    TopologyFileLibrary::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileLibrary>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileLibrary);

    TopologyFileInstances::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileInstances>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileInstances);

    TopologyFileBindings::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileBindings>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileBindings);

    TopologyFileRoutes::RegisterMetaTypes();
    QMetaType::registerComparators<QVector<TopologyFileRoutes>>();
    QVARIANTLISTHELPER_REGISTER_VARIANTLIST_TO_VARIANT_CONVERTER(QVector,
                                                                 TopologyFileRoutes);

    qRegisterMetaType<TopologyFile>();
    QMetaType::registerComparators<TopologyFile>();
}

const QMetaObject *TopologyFile::metaObject() const
{
    return &staticMetaObject;
}
