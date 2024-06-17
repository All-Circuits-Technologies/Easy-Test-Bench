# SPDX-FileCopyrightText: 2020 - 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++17, if not returns an error when compiling
c++17 | c++2* {
    # At least C++17 - OK
} else {
    error($${TARGET} requires at least c++17)
}

!contains(DEFINES, THREAD_BMS_LIB) : error($$${TARGET} requires THREAD_BMS_LIB)
!contains(DEFINES, STRING_BMS_LIB) : error($$${TARGET} requires STRING_BMS_LIB)
!contains(DEFINES, IMPORT_QTSERIALLINKLIB) : error($$${TARGET} requires IMPORT_QTSERIALLINKLIB)
!contains(DEFINES, TEST_BED_CORE) : error($$${TARGET} requires TEST_BED_CORE)

PLUGIN_SERIAL_LINK_LIB_ROOT = $$absolute_path(.)

DEFINES *= PLUGIN_SERIAL_LINK_LIB

INCLUDEPATH *= $$PLUGIN_SERIAL_LINK_LIB_ROOT

# Common classes
HEADERS *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/aseriallinkwriteread.hpp
SOURCES *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/aseriallinkwriteread.cpp
HEADERS *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/acallscriptviaseriallink.hpp
SOURCES *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/acallscriptviaseriallink.cpp
HEADERS *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/serialpluginlibconstants.hpp

# Those are the types used in classes
HEADERS *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/types/seriallinkreceivedreplytype.hpp
SOURCES *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/types/seriallinkreceivedreplytype.cpp

# Those are the process classes
HEADERS *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/processes/messagereceptionprocess.hpp
SOURCES *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/processes/messagereceptionprocess.cpp

# Those are the utilities classes
HEADERS *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/utilities/serialmodulesutility.hpp
SOURCES *= $$PLUGIN_SERIAL_LINK_LIB_ROOT/utilities/serialmodulesutility.cpp
