##
# Top-level CMakeLists.txt
#
# cmake -G"Eclipse CDT4 - Unix Makefiles"
# -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE -DCMAKE_ECLIPSE_VERSION=4.4
# -DCMAKE_TOOLCHAIN_FILE=/home/ruschi/Coding/Toolchain-xxx.cmake
#
##

cmake_minimum_required (VERSION 3.3)

# Before the Project!
SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_ERROR_DEPRECATED ON)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project Name + Languages
project (FreeRTOS_Tiva_Test CXX C)
# Some arbitrary project description - only used in doxygen and cpack
set(PROJECT_DESCRIPTION "Testproject for TivaWare+FreeRTOS")

# Some arbitrary hand-maintained version
set(PROJECT_MAJOR_VERSION 1)
set(PROJECT_MINOR_VERSION 0)
set(PROJECT_PATCH_VERSION 0)
set(PROJECT_VERSION  ${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_PATCH_VERSION})

message(STATUS "")
message(STATUS "Running ${CMAKE_COMMAND} ${CMAKE_VERSION} on ${CMAKE_PROJECT_NAME} v${PROJECT_VERSION}" )


#-------------------------------------------------------------------------------
# User Configuration + Defaults
#-------------------------------------------------------------------------------

IF(NOT CMAKE_BUILD_TYPE)
  #Default to Release-Build
  SET(CMAKE_BUILD_TYPE Release
    CACHE STRING "Choose the type of build : Debug Release"
    FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)


# User did not provide install prefix via cmake commandline
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  SET(CMAKE_INSTALL_PREFIX ${STAGING_DIR} )
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#-------------------------------------------------------------------------------
# Print Summary
#-------------------------------------------------------------------------------
message(STATUS "------------- Config ${PROJECT_NAME} -------------" )
message(STATUS " Build-Type:\t\t ${CMAKE_BUILD_TYPE} " )
message(STATUS " CPU Family:\t\t ${PROCESSOR_FAMILY} ")
message(STATUS " CPU:\t\t ${TIVA_PART_NAME} ")
message(STATUS " Install Prefix:\t ${CMAKE_INSTALL_PREFIX} ")
message(STATUS "" )

#-------------------------------------------------------------------------------
# Input configuration
#-------------------------------------------------------------------------------

# top-level include directory will be searched in all builds
SET(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

#-------------------------------------------------------------------------------
# Output  configuration
#-------------------------------------------------------------------------------

# User did not provide install prefix via cmake commandline
if( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  SET(CMAKE_INSTALL_PREFIX "/usr" )
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Make debug libs visible per default ->  SET_TARGET_PROPERTIES
SET(CMAKE_DEBUG_POSTFIX "_dbg")
# (CMAKE_BINARY_DIR = where cmake was invoked and found project())
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY	"${PROJECT_BINARY_DIR}/bin/")
# Folder for libraries (during build)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY	"${PROJECT_BINARY_DIR}/lib/")
# Put Archives + Libraries together
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH  "Installation directory for header files")
set(INSTALL_CMAKE_DIR ${INSTALL_LIB_DIR}/cmake/ CACHE PATH
    "Installation directory for CMake files")
set(INSTALL_DOC_DIR "doc/${PROJECT_NAME}" CACHE PATH  "Installation directory for doxygen docs")



#-------------------------------------------------------------------------------
# Sub-Projects to build each with its own CMakelists.txt
#-------------------------------------------------------------------------------

# Sub-Projects
add_subdirectory(src/)



MESSAGE(STATUS "** end CMakeLists.txt **\n")
