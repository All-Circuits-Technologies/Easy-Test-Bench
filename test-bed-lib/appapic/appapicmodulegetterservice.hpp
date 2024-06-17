// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include <QObject>
#include <QString>

class AppApiCCatalogModel;
class AppApiCModuleModel;


/** @brief Service to get the list of plugins */
class AppApiCModuleGetterService : public QObject
{

    Q_OBJECT

    public:
        /** @brief class constructor*/
        explicit AppApiCModuleGetterService(QObject *parent = nullptr);

    public:
        /** @brief get the list of sequence modules
         *  @return a catalog with the list of plugins or nullptr if something went wrong */
        AppApiCCatalogModel getSequenceModules();

        /** @brief reload the banks and get the list of sequence modules
         *  @return a catalog with the list of plugins */
        AppApiCCatalogModel reloadSequenceModules();

    private:
        /** @brief get a module and its params from its plugin and name
         *  @param bankName The name of the plugin
         *  @param moduleName The name of the module
         *  @param module The module model
         *  @return false if something went wrong */
        bool getModule(const QString &bankName,
                       const QString &moduleName,
                       AppApiCModuleModel &module);

    private:
        /** @brief The sequenceToken to create sequence instance modules
         *  @note any value will do */
        static const constexpr char *SequenceToken = "";

};
