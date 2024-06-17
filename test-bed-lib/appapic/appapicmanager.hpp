// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#include <QMutex>
#include <QSemaphore>

struct AppApiCCatalogStruct;
class AppApiCJsonCheckerService;
class AppApiCModuleGetterService;


class AppApiCManager
{

    private:
        /** @brief class constructor*/
        explicit AppApiCManager();

    public:
        /** @brief class destructor */
        virtual ~AppApiCManager();

    public:
        /** @brief get the list of sequence modules
         *  @return a catalog with the list of plugins or nullptr if something went wrong */
        AppApiCCatalogStruct *getSequenceModules();

        /** @brief reload the banks and get the list of sequence modules
         *  @return a catalog with the list of plugins or nullptr if something went wrong */
        AppApiCCatalogStruct *reloadSequenceModules();

        /** @brief verify if the json sequence is valid
         *  @param json The json stringified to verify
         *  @param error Description of the error, null if no problem occurs
         *  @param nodeName The name of the incorrect node, may be null depending on the error
         *  @return True If no problem occurs */
        bool checkJson(char *json, char **error, char **nodeName);

        /** @brief get the version of test-bed-lib
         *  @param version The version of test-bed-lib
         *  @return True if no problem occurs */
        bool getVersion(char **version);

    public:
        /** @brief Get singleton instance.
         *  return singleton instance */
        static AppApiCManager& instance();

    private:
        /** @brief create the Qt thread and instanciate all members */
        void init();

    private:
        /** @brief function for the thread with QCoreApplication
         *  @param  pointer to the function param
         *  @return QCoreApplication::exec()*/
        static unsigned long qtThreadFunc(void *param);

    private:
        static const constexpr int Argc = 1;
        static const constexpr char *Argv[] = {""};
        static const constexpr int ThreadReadyTimeoutInMs = 5000;
        static const constexpr char *Version = "x.x.x";
        static AppApiCManager *_instance;

    private:
        QRecursiveMutex _mutex;
        QSemaphore _semReady;
        bool _ready{false};
        AppApiCModuleGetterService *_moduleGetter{nullptr};
        AppApiCJsonCheckerService *_jsonChecker{nullptr};

};
