# SPDX-FileCopyrightText: 2019 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2019 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

EXAMPLES_ROOT = $$absolute_path(.)

# Text files
OTHER_FILES *= $$files($${EXAMPLES_ROOT}/*.txt, true)
OTHER_FILES *= $$files($${EXAMPLES_ROOT}/*.md, true)


# Project examples are a set of JSON files with custom file extensions:
# - "tb" files    (Test Bed)
# - "tbs" files   (Test Bed Sequence)
# - "tbt" files   (Test Bed Topology)
# - "tbtc" files  (Test Bed Topology Classes)
# - "tbti" files  (Test Bed Topology Instances)
# - "tbtb" files  (Test Bed Topology Bindings)
# - "tbtr" files  (Test Bed Topology Routes)
OTHER_FILES *= $$files($${EXAMPLES_ROOT}/*.tb, true)
OTHER_FILES *= $$files($${EXAMPLES_ROOT}/*.tbs, true)
OTHER_FILES *= $$files($${EXAMPLES_ROOT}/*.tbt, true)
OTHER_FILES *= $$files($${EXAMPLES_ROOT}/*.tbtc, true)
OTHER_FILES *= $$files($${EXAMPLES_ROOT}/*.tbti, true)
OTHER_FILES *= $$files($${EXAMPLES_ROOT}/*.tbtb, true)
OTHER_FILES *= $$files($${EXAMPLES_ROOT}/*.tbtr, true)
