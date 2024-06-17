<!--
SPDX-FileCopyrightText: 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# yq

## Presentation

To convert YAML files to JSON files (needed by the app), we use yq app. If used, the app has to be downloaded and put in this folder.

## Download and usage

Thanks to Jenkins, gets the last version of yq from github for your production environment: https://github.com/mikefarah/yq/releases

The path and name of the file has to be set in the ini file. If not set, the executable file doesn't exist or the executable doesn't work, the app wouldn't use YAML file.
