<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

# Intel Hex File plugin

## Table of contents

- [Intel Hex File plugin](#intel-hex-file-plugin)
  - [Table of contents](#table-of-contents)
  - [Presentation](#presentation)
  - [Modules list](#modules-list)
    - [UpdateHexFile](#updatehexfile)
      - [Presentation](#presentation-1)
      - [Schematic representation](#schematic-representation)

## Presentation

This plugin contains modules which help to manage "hex" files.

## Modules list

### UpdateHexFile

#### Presentation

This module is useful to update the content of a hex file.

The address is given by parameter and the value by input.

#### Schematic representation

```mermaid
flowchart LR
    subgraph "UpdateHexFile"
        direction LR
        subgraph "Inputs"
            direction TB
            subgraph Input1[hexFilePath]
                direction TB
                Input1Info["Type: string\nDesc: The path of the intel hex file to update"]
            end
            subgraph Input2[dataValueToUpdate]
                direction TB
                Input2Info["Type: string\nDesc: This is the data to update on the hex file. The date has to be a hexadecimal number"]
            end
            Input1 ~~~ Input2
        end
        subgraph "Parameters"
            direction TB
            subgraph Param1[addressToUpdate]
                direction TB
                Param1Info["Type: number\nRequired: true\nMin: 0; max: maxUint16; step: 1\nDesc: The data is updated from this address"]
            end
        end
        subgraph "Outputs"
        end
        Inputs --> Parameters
        Parameters --> Outputs
    end
```
