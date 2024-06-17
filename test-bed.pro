# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2019 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# ------------------------------------------------------------------
# test-bed.pro - COPYRIGHT (C) 2019 BMS CIRCUITS
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# This project file is a super-project over all Test Bed software pieces.
#
# It includes those sub-projects as well as few generic extra files
# ------------------------------------------------------------------

TEMPLATE = subdirs

ROOT = $$absolute_path(.)
ACTLIBS = $$absolute_path($$ROOT/actlibs)
QTLIBS = $$absolute_path($$ACTLIBS/qtlibs)

SUBDIRS *= plugins
SUBDIRS *= test-bed-lib                   # Test bed main library
SUBDIRS *= test-bed-app
SUBDIRS *= qtvisacomlib
SUBDIRS *= qtseriallinklib
SUBDIRS *= qtpeakcanlib
SUBDIRS *= test-bed-utest                 # Test bed unit tests

qtvisacomlib.subdir = $$QTLIBS/qtvisacomlib
qtseriallinklib.subdir = $$QTLIBS/qtseriallinklib
qtpeakcanlib.subdir = $$QTLIBS/qtpeakcanlib

test-bed-app.subdir = $$ROOT/test-bed-app
test-bed-app.depends = test-bed-lib

plugins.subdir = $$ROOT/plugins
plugins.depends = qtvisacomlib qtseriallinklib qtpeakcanlib
