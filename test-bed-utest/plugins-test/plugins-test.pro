# SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2020 Matthieu Seigeot <matthieu.seigeot@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# ------------------------------------------------------------------
# plugins-test.pro - COPYRIGHT (C) 2019 BMS CIRCUITS
# ------------------------------------------------------------------

# ------------------------------------------------------------------
# This project file include all the plugins usefull for unit tests
# ------------------------------------------------------------------

TEMPLATE = subdirs

CONFIG *= ordered

SUBDIRS *= $$files(plugin-*, false)

unix {
    target.path = /opt/testbed
    INSTALLS += target
}
