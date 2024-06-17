# SPDX-FileCopyrightText: 2019 - 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
# SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# ------------------------------------------------------------------
# test-bed-utest.pro - COPYRIGHT (C) 2019 BMS CIRCUITS
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# This project file include all unit tests found in its folder
# ------------------------------------------------------------------

TEMPLATE = subdirs

SUBDIRS *= $$files(test-bed-utest-*, false)

SUBDIRS *= plugins-test
