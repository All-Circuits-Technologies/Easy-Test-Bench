<!--
SPDX-FileCopyrightText: 2019 - 2020 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>

SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1
-->

Fake-Project is a fake TestBed project for example and testing purposes.

Its architecture is as this:
| file                         | Kind of file                    | Content of the file                                                                        |
| ---------------------------- | ------------------------------- | ------------------------------------------------------------------------------------------ |
| fake-project.tb              | TestBed project files           | Main entry point, points to database, sequence and topology files                          |
| topology.tbt                 | TestBed Topology file           | Topology entry point, points to topology classes, instances, bindings and route hint files |
| topology/001-classes1.tbtc   | TestBed Topology Classes file   | Optional set of custom devices templates                                                   |
| topology/002-instances1.tbtc | TestBed Topology Instances file | Actual devices object contained in the topology                                            |
| topology/003-bindings1.tbtc  | TestBed Topology Bindings file  | Wires between objects                                                                      |
| topology/004-routes1.tbtc    | TestBed Topology Route file     | Optional prefered wires (hints) to join to pads                                            |
