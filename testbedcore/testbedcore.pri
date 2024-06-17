# SPDX-FileCopyrightText: 2019 - 2022 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
# SPDX-FileCopyrightText: 2021 Raphael Aurejac <raphael.aurejac@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

# Test if the main application is using at least C++11, if not returns an error when compiling
c++1* | c++2* {
    # At least C++11 - OK
} else {
    error($${TARGET} requires at least c++11)
}

!contains(DEFINES, WAIT_BMS_LIB) : error($${TARGET} requires WAIT_BMS_LIB)
!contains(DEFINES, COLLECTION_BMS_LIB) : error($${TARGET} requires COLLECTION_BMS_LIB)
!contains(DEFINES, LOGS_BMS_LIB) : error($${TARGET} requires LOGS_BMS_LIB)

DEFINES *= TEST_BED_CORE

TEST_BED_CORE_LIB_ROOT = $$absolute_path(.)

INCLUDEPATH *= $$TEST_BED_CORE_LIB_ROOT

SOURCES *= $$TEST_BED_CORE_LIB_ROOT/customevents.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/customevents.hpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/testbedglobal.hpp

# Logs elements
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/logs/sequencelogcategory.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/logs/sequencelogcategory.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/logs/sequencelogdata.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/logs/sequencelogdata.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/logs/sequencelogformatter.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/logs/sequencelogformatter.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/logs/sequencelogformattype.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/logs/sequencelogformattype.hpp

# Types elements
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/types/qtdateformathelper.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/types/qtdateformathelper.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/types/qttimespechelper.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/types/qttimespechelper.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/types/sequencestatetype.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/types/sequencestatetype.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/types/stopaskedtype.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/types/stopaskedtype.cpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/types/typemanaged.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/types/typemanaged.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/types/unit.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/types/unit.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/types/unittype.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/types/unittype.hpp

# Parsers elements
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/parsers/bankjsonformatter.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/parsers/bankjsonformatter.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/parsers/bankjsonparamvalidator.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/parsers/bankjsonparamvalidator.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/parsers/bankjsonparser.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/parsers/bankjsonparser.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/parsers/commonjsonparser.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/parsers/commonjsonparser.hpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/parsers/jsonparsersconstants.hpp

# Utility elements
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/utility/pauseandstophelper.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/utility/pauseandstophelper.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/utility/sequencefileshelper.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/utility/sequencefileshelper.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/utility/sequencestatehelper.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/utility/sequencestatehelper.hpp

# Errors
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/errors/errorcause.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/errors/errorcause.hpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/errors/errordescriptor.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/errors/errortype.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/errors/errortype.hpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/errors/errorlayer.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/errors/fileloadingerrorhandler.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/errors/fileloadingerrorhandler.hpp

#helpers
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/errors/helpers/fileloadingerrorhelper.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/errors/helpers/fileloadingerrorhelper.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/errors/helpers/fileloadingerrorcausehelper.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/errors/helpers/fileloadingerrorcausehelper.hpp
SOURCES *= $$TEST_BED_CORE_LIB_ROOT/errors/helpers/fileloadingerrortypehelper.cpp
HEADERS *= $$TEST_BED_CORE_LIB_ROOT/errors/helpers/fileloadingerrortypehelper.hpp
