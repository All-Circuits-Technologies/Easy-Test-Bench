<!--
SPDX-FileCopyrightText: 2024 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

<!-- Optionnal header with this format:
     --------------------------------------------------------------
     |            |                                  |            |
     |  ACT LOGO  |         SW-Project-Name          |  CUSTOMER  |
     |            |                                  |    LOGO    |
     |            |                                  |            |
     --------------------------------------------------------------
  -->

## Firmware/Software Release Notes

<!-- Information about the person who published the release notes  -->

- **Author**  : Benoît Rolandeau
- **Company** : AC Technologies - BMS Circuits


## Firmware/Software Version

<!-- Lists all releases delivered to the customer - must be listed from most recent to oldest  -->

| **Version No** | **Notes**                                                                  | **Delivery Date** |
| -------------- | -------------------------------------------------------------------------- | ----------------- |
| 1.3.0          | Add more documentation on the code, fix some problems and add new modules. | 2024-03-20        |
| 1.2.0          | Add Peak Can plugin and new modules.                                       | 2023-12-15        |
| 1.1.1          | Fix test bed crashes.                                                      | 2022-12-16        |
| 1.1.0          | Update HMI and fix bugs                                                    | 2022-11-21        |
| 1.0.0          | First release of the test bed in production                                | 2021-04-16        |

## Table of Contents

<!-- The table of contents is updated automatically with vscode -->
<!-- To do this, you need the Markdown All In One extension. -->
<!-- Simply save your changes and the table of contents will be updated automatically. -->
<!-- If it doesn't work: CTRL+SHIFT+P ==> Markdown All In One: create table of contents -->

- [Firmware/Software Release Notes](#firmwaresoftware-release-notes)
- [Firmware/Software Version](#firmwaresoftware-version)
- [Table of Contents](#table-of-contents)
- [Introduction](#introduction)
- [Overview](#overview)
- [Software Deliveries details](#software-deliveries-details)
  - [SW 1.3.0](#sw-130)
    - [Overview](#overview-1)
    - [Delivery Type](#delivery-type)
    - [Input Documents](#input-documents)
    - [Output Documents](#output-documents)
    - [Compatibilties and configurations](#compatibilties-and-configurations)
    - [ChangeLog](#changelog)
      - [New Features:](#new-features)
      - [Bug Fixes:](#bug-fixes)
  - [SW 1.2.0](#sw-120)
    - [Overview](#overview-2)
    - [Delivery Type](#delivery-type-1)
    - [Input Documents](#input-documents-1)
    - [Output Documents](#output-documents-1)
    - [Compatibilties and configurations](#compatibilties-and-configurations-1)
    - [ChangeLog](#changelog-1)
      - [New Features:](#new-features-1)
      - [Bug Fixes:](#bug-fixes-1)
  - [SW 1.1.1](#sw-111)
    - [Overview](#overview-3)
    - [Delivery Type](#delivery-type-2)
    - [Input Documents](#input-documents-2)
    - [Output Documents](#output-documents-2)
    - [Compatibilties and configurations](#compatibilties-and-configurations-2)
    - [ChangeLog](#changelog-2)
      - [New Features:](#new-features-2)
      - [Bug Fixes:](#bug-fixes-2)
  - [SW 1.1.0](#sw-110)
    - [Overview](#overview-4)
    - [Delivery Type](#delivery-type-3)
    - [Input Documents](#input-documents-3)
    - [Output Documents](#output-documents-3)
    - [Compatibilties and configurations](#compatibilties-and-configurations-3)
    - [ChangeLog](#changelog-3)
      - [New Features:](#new-features-3)
      - [Bug Fixes:](#bug-fixes-3)
  - [SW 1.0.0](#sw-100)
    - [Overview](#overview-5)
    - [Delivery Type](#delivery-type-4)
    - [Input Documents](#input-documents-4)
    - [Output Documents](#output-documents-4)
    - [Compatibilties and configurations](#compatibilties-and-configurations-4)
    - [ChangeLog](#changelog-4)
      - [New Features:](#new-features-4)
      - [Bug Fixes:](#bug-fixes-4)


## Introduction

<!-- Provides informations about release type and release notes -->

This document contains the release notes of the delivered software.

<u> Important: </u>
Releases, that are marked as:

- “Developer / intermediate version” are not intended to be used in final product and must be used for test only.
- "Release" is the official sw version.


## Overview

<!-- Provides informations about release Notes contents -->

This release notes are valid for the software EasyTestBench.
Its objective is to provide an overview of the delivered software:

- Make reference to related documents.
- Make reference to the software starting base [SW Architecture, SW Requirements, ... ].
- Show possible hardware dependencies.
- Describe special features or other restrictions.
- List resolved bugs.


## Software Deliveries details

<!-- List and provides details for each releases delivered to the client -->
<!-- It's like an improved changelog -->

### SW 1.3.0

<details>

#### Overview

<!-- Provides a detailed view of:
      - all the elements developed in the release
      - important information to communicate to the customer
      - what is functional and what is not -->

* Overview Here

#### Delivery Type

<!-- Defines if the release is an official release or a release candidate -->

- [ ] Developer / intermediate version
- [x] Release


#### Input Documents

<!-- Lists all the documents used to develop the code -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               |       |

#### Output Documents

<!-- Lists all the documents generated for the release -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               | -     |

#### Compatibilties and configurations

<!-- Provides informations about SW/HW/MECA/... compatibility -->

| Description                  | Compatible version | Notes |
| ---------------------------- | ------------------ | ----- |
| test-bed-app                 | 1.0.3              |       |
| test-bed-lib                 | 1.1.3              |       |
| analyse-results-plugin       | 1.1.2              |       |
| atmel-program-plugin         | 1.1.1              |       |
| file-browser-plugin          | 1.1.1              |       |
| intel-hex-file-plugin        | 1.1.1              |       |
| logging-plugin               | 1.1.2              |       |
| manage-storages-plugin       | 1.1.1              |       |
| measure-compare-test-plugin  | 1.1.2              |       |
| peak-can-plugin              | 1.0.1              |       |
| read-settings-plugin         | 1.1.2              |       |
| scan-data-matrix-plugin      | 1.1.2              |       |
| segger-jlink-plugin          | 1.1.1              |       |
| serial-link-plugin           | 1.1.1              |       |
| time-plugin                  | 1.1.2              |       |
| variable-compare-test-plugin | 1.1.2              |       |
| variable-convert-plugin      | 1.1.1              |       |
| variable-operations-plugin   | 1.1.1              |       |
| vds200-plugin                | 1.0.1              |       |
| visa-com-plugin              | 1.1.1              |       |
| qtpeakcanlib                 | 1.0.0              |       |
| qtseriallinklib              | 1.1.0              |       |
| qtvisacomlib                 | 1.0.0              |       |


#### ChangeLog

<!-- New features and bug fixes are listed with there redmine reference and details [#ticket-ref] - title of the ticket -->

##### New Features:

- #17725: [Tsk] Ajouter un fichier de traduction aux applications et modules
- #17103: [Tsk] Utiliser la librairie Qt partagé en sous module
- #15844: [US] [UST] Création d'un module pour tester toutes les GPIO
- #15810: [Exg] Modification du module de flashage JLink pour prendre des fichiers JLink

##### Bug Fixes:

- #16983: [Lvl-2] Mauvais serial number ne créé pas de fichier de logs
- #17171: [Lvl-3] Non destruction lors d'un stop
- #17696: [Lvl-3] Manque des traductions dans la vue des inputs pour les lettres de catégories de logs

</details>

### SW 1.2.0

<details>

#### Overview

<!-- Provides a detailed view of:
      - all the elements developed in the release
      - important information to communicate to the customer
      - what is functional and what is not -->

* Overview Here

#### Delivery Type

<!-- Defines if the release is an official release or a release candidate -->

- [ ] Developer / intermediate version
- [x] Release


#### Input Documents

<!-- Lists all the documents used to develop the code -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               |       |

#### Output Documents

<!-- Lists all the documents generated for the release -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               | -     |

#### Compatibilties and configurations

<!-- Provides informations about SW/HW/MECA/... compatibility -->

| Description                  | Compatible version | Notes |
| ---------------------------- | ------------------ | ----- |
| test-bed-app                 | 1.0.2              |       |
| test-bed-lib                 | 1.1.2              |       |
| analyse-results-plugin       | 1.1.1              |       |
| atmel-program-plugin         | 1.1.0              |       |
| file-browser-plugin          | 1.1.0              |       |
| intel-hex-file-plugin        | 1.1.0              |       |
| logging-plugin               | 1.1.1              |       |
| manage-storages-plugin       | 1.1.0              |       |
| measure-compare-test-plugin  | 1.1.1              |       |
| peak-can-plugin              | 1.0.0              |       |
| read-settings-plugin         | 1.1.1              |       |
| scan-data-matrix-plugin      | 1.1.1              |       |
| segger-jlink-plugin          | 1.1.0              |       |
| serial-link-plugin           | 1.1.0              |       |
| time-plugin                  | 1.1.1              |       |
| variable-compare-test-plugin | 1.1.1              |       |
| variable-convert-plugin      | 1.1.0              |       |
| variable-operations-plugin   | 1.1.0              |       |
| vds200-plugin                | 1.0.0              |       |
| visa-com-plugin              | 1.1.0              |       |
| qtpeakcanlib                 | 1.0.0              |       |
| qtseriallinklib              | 1.0.0              |       |
| qtvisacomlib                 | 0.1.1              |       |


#### ChangeLog

<!-- New features and bug fixes are listed with there redmine reference and details [#ticket-ref] - title of the ticket -->

##### New Features:

- #15804: [Exg] Creating of a common plugin library for CAN and CAN FD management
- #15805: [Exg] Creating of a test module to manage the sending of CAN requests with expected response
- #15806: [Exg] Creating a module to call commands in a remote console
- #16972: [Tsk] Update the test bench logo

##### Bug Fixes:

- #16968: [Lvl-1] Problem with Serial and Can manager initialization

</details>

### SW 1.1.1

<details>

#### Overview

<!-- Provides a detailed view of:
      - all the elements developed in the release
      - important information to communicate to the customer
      - what is functional and what is not -->

* Overview Here

#### Delivery Type

<!-- Defines if the release is an official release or a release candidate -->

- [ ] Developer / intermediate version
- [x] Release


#### Input Documents

<!-- Lists all the documents used to develop the code -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               |       |

#### Output Documents

<!-- Lists all the documents generated for the release -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               | -     |

#### Compatibilties and configurations

<!-- Provides informations about SW/HW/MECA/... compatibility -->

| Description                  | Compatible version | Notes |
| ---------------------------- | ------------------ | ----- |
| test-bed-app                 | 1.0.1              |       |
| test-bed-lib                 | 1.1.0              |       |
| analyse-results-plugin       | 1.1.0              |       |
| atmel-program-plugin         | 1.1.0              |       |
| file-browser-plugin          | 1.1.0              |       |
| intel-hex-file-plugin        | 1.1.0              |       |
| logging-plugin               | 1.1.0              |       |
| manage-storages-plugin       | 1.1.0              |       |
| measure-compare-test-plugin  | 1.1.0              |       |
| read-settings-plugin         | 1.1.0              |       |
| scan-data-matrix-plugin      | 1.1.0              |       |
| segger-jlink-plugin          | 1.0.0              |       |
| serial-link-plugin           | 1.0.0              |       |
| time-plugin                  | 1.1.0              |       |
| variable-compare-test-plugin | 1.1.0              |       |
| variable-convert-plugin      | 1.1.0              |       |
| variable-operations-plugin   | 1.1.0              |       |
| visa-com-plugin              | 1.1.0              |       |
| qtvisacomlib                 | 0.1.1              |       |


#### ChangeLog

<!-- New features and bug fixes are listed with there redmine reference and details [#ticket-ref] - title of the ticket -->

##### New Features:

None

##### Bug Fixes:

- #13560: Application crashes 2 times per hour + sequence duration much longer than the previous one
- #13506: “Plugins” tab grayed out when running a sequence

</details>

### SW 1.1.0

<details>

#### Overview

<!-- Provides a detailed view of:
      - all the elements developed in the release
      - important information to communicate to the customer
      - what is functional and what is not -->

* Overview Here

#### Delivery Type

<!-- Defines if the release is an official release or a release candidate -->

- [ ] Developer / intermediate version
- [x] Release


#### Input Documents

<!-- Lists all the documents used to develop the code -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               |       |

#### Output Documents

<!-- Lists all the documents generated for the release -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               | -     |

#### Compatibilties and configurations

<!-- Provides informations about SW/HW/MECA/... compatibility -->

| Description                  | Compatible version | Notes |
| ---------------------------- | ------------------ | ----- |
| test-bed-app                 | 1.0.0              |       |
| test-bed-lib                 | 1.1.0              |       |
| analyse-results-plugin       | 1.1.0              |       |
| atmel-program-plugin         | 1.1.0              |       |
| file-browser-plugin          | 1.1.0              |       |
| intel-hex-file-plugin        | 1.1.0              |       |
| logging-plugin               | 1.1.0              |       |
| manage-storages-plugin       | 1.1.0              |       |
| measure-compare-test-plugin  | 1.1.0              |       |
| read-settings-plugin         | 1.1.0              |       |
| scan-data-matrix-plugin      | 1.1.0              |       |
| segger-jlink-plugin          | 1.0.0              |       |
| serial-link-plugin           | 1.0.0              |       |
| time-plugin                  | 1.1.0              |       |
| variable-compare-test-plugin | 1.1.0              |       |
| variable-convert-plugin      | 1.1.0              |       |
| variable-operations-plugin   | 1.1.0              |       |
| visa-com-plugin              | 1.1.0              |       |
| qtvisacomlib                 | 0.1.1              |       |


#### ChangeLog

<!-- New features and bug fixes are listed with there redmine reference and details [#ticket-ref] - title of the ticket -->

##### New Features:

- #9622: [ES-9311] Sequence loading error handling view
- #9522: [ES-9504] Update widget GUIs requested by plugins
- #9521: [ES-9504] Display widgets requested by plugins
- #9516: [ES-9504] View the number of sequences performed
- #9514: [ES-9504] Stop a sequence
- #9513: [ES-9504] Pause a sequence
- #9512: [ES-9504] Start a sequence at infinity
- #9505: [ES-9504] View sequence logs
- #9480: [ES-9311] Project loading error handling view
- #9477: [ES-9311] Opening a new project
- #9476: [ES-9312] Selecting and loading a project
- #9475: [ES-9312] Viewing the list of most recent projects
- #9313: [US-9311] Creation of the loading page of a sequence (linked to a project)
- #9254: [US-9252] Sign out
- #9253: [US-9252] Display of current user
- #9251: [ES-9250] Adding an option for anonymous login
- #9249: [ES-9250] Managing authentication to a guest account
- #9248: [ES-8916] Protection of the database against inconsistencies
- #9133: [ES-9250] Managing authentication to a given user account

##### Bug Fixes:

- #9537: Impossible to have two sequences with the same name and different paths within the same project
- #9841: Application crash when closing a sequence
- #11575: Fix MinGW compilation

</details>

### SW 1.0.0

<details>

#### Overview

<!-- Provides a detailed view of:
      - all the elements developed in the release
      - important information to communicate to the customer
      - what is functional and what is not -->

* Overview Here

#### Delivery Type

<!-- Defines if the release is an official release or a release candidate -->

- [ ] Developer / intermediate version
- [x] Release


#### Input Documents

<!-- Lists all the documents used to develop the code -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               |       |

#### Output Documents

<!-- Lists all the documents generated for the release -->

| Description | Document Identification Version | Notes |
| ----------- | ------------------------------- | ----- |
| -           | -                               | -     |

#### Compatibilties and configurations

<!-- Provides informations about SW/HW/MECA/... compatibility -->

| Description                  | Compatible version | Notes |
| ---------------------------- | ------------------ | ----- |
| test-bed-lib                 | 1.0.0              |       |
| test-bed-simple-gui          | 1.0.0              |       |
| analyse-results-plugin       | 1.0.0              |       |
| atmel-program-plugin         | 1.0.0              |       |
| file-browser-plugin          | 1.0.0              |       |
| intel-hex-file-plugin        | 1.0.0              |       |
| logging-plugin               | 1.0.0              |       |
| manage-storages-plugin       | 1.0.0              |       |
| measure-compare-test-plugin  | 1.0.0              |       |
| read-settings-plugin         | 1.0.0              |       |
| scan-data-matrix-plugin      | 1.0.0              |       |
| segger-jlink-plugin          | 1.0.0              |       |
| serial-link-plugin           | 1.0.0              |       |
| time-plugin                  | 1.0.0              |       |
| variable-compare-test-plugin | 1.0.0              |       |
| variable-convert-plugin      | 1.0.0              |       |
| variable-operations-plugin   | 1.0.0              |       |
| visa-com-plugin              | 1.0.1              |       |
| qtvisacomlib                 | 0.1.1              |       |


#### ChangeLog

<!-- New features and bug fixes are listed with there redmine reference and details [#ticket-ref] - title of the ticket -->

##### New Features:

First release

##### Bug Fixes:

None

</details>
