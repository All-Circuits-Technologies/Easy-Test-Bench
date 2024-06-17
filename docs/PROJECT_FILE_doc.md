<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Project file documentation

## Table of contents

- [Project file documentation](#project-file-documentation)
  - [Table of contents](#table-of-contents)
  - [Details](#details)

## Details

A project file is a JSON or YAML file which has the extension:

- `*.tb` if it's written in JSON
- `*.ytb` if it's written in YAML

`tb` is for "**t**est **b**ench project file"

It contains at least the following attributes:

```json
{
    "projectName": "Simple sequence tests",
    "sequenceFilesPath": [
        "1_simple_timer_test.tbs",
        "2_buggy_sequence.tbs",
        "3_scan_data_matrix_test.tbs",
        "4_multiple_scan_data_matrix_test.tbs",
        "5_multiple_plugins_views.tbs",
        "6_serial_link_test.tbs"
    ],
    "topologyFilesPath": []
}
```

With:

- `projectName`: the name of the project
- `sequenceFilesPath`: the relative path (from the project file path) of the child sequence files
- `topologyFilesPath`: the relative path (from the project file path) of the child topology files
