// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

#define EXTERNCEXPORT  __declspec(dllexport)
#ifdef __cplusplus
#define EXTERNCEXPORT  extern "C" __declspec(dllexport)
#endif

#include "stdbool.h"

#include "models/appapiccatalogstruct.h"


/** @brief get the list of sequence modules
 *  @note catalog should be freed using freeCatalog
 *  @return a catalog with the list of plugins or nullptr if something went wrong */
EXTERNCEXPORT AppApiCCatalogStruct *getSequenceModules();

/** @brief reload the banks and get the list of sequence modules
 *  @note catalog should be freed using freeCatalog
 *  @return a catalog with the list of plugins or nullptr if something went wrong */
EXTERNCEXPORT AppApiCCatalogStruct *reloadSequenceModules();

/** @brief verify if the json sequence is valid
 *  @param json The json stringified to verify
 *  @param error Description of the error, null if no problem occurs
 *  @param nodeName The name of the incorrect node, may be null depending on the error
 *  @return True If no problem occurs */
EXTERNCEXPORT bool checkJson(char *json, char **error, char **nodeName);

/** @brief get the version of test-bed-lib
 *  @param version The version of test-bed-lib
 *  @return True if no problem occurs */
EXTERNCEXPORT bool getVersion(char **version);

/** @brief free the memory allocated for an AppApiCCatalogStruct
 *  @param catalog pointer to the struct */
EXTERNCEXPORT void freeCatalog(AppApiCCatalogStruct *catalog);

/** @brief free the memory allocated for a char*
 *  @param pointer to the char* */
EXTERNCEXPORT void freeString(char *p);
