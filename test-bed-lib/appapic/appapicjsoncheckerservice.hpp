// SPDX-FileCopyrightText: 2024 Hugo Landre <hugo.landre@allcircuits.com>
//
// SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1


/** @brief Service to verify a json sequence */
class AppApiCJsonCheckerService
{

    public:
        /** @brief class constructor*/
        explicit AppApiCJsonCheckerService();

    public:
        /** @brief verify if the json sequence is valid
         *  @param json The json stringified to verify
         *  @param error description of the error, null if no problem occurs
         *  @param nodeName The name of the incorrect node, may be null depending on the error
         *  @return True If no problem occurs */
        bool checkJson(char *json, char **error, char **nodeName);

    private:
        static const constexpr char *ErrNotAnArray = "json is not an array";
        static const constexpr char *ErrParsingSequence = "failed to parse json sequence";
};
