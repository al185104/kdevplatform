# - Try to find Boost include dirs and libraries
# Usage of this module as follows:
#
#     SET( Boost_REQUIRED_VERSION "1.34.1" )
#     FIND_PACKAGE( Boost COMPONENTS date_time filesystem iostreams )
#
# The Boost_REQUIRED_VERSION variable will be used to determine part of the
# boost include dir and library names and will also be used to do a version
# check. Its expected that this variable always has the form 
# <number>.<number>.<number>. The default version to be checked for if this
# variable is not set will be 1.34.1
#
# Variables used by this module, they can change the default behaviour:
#  Boost_USE_NONMULTITHREAD      Can be set to TRUE to use the non-multithreaded
#                                boost libraries.
#
# Variables defined by this module:
#
#  Boost_FOUND                System has Boost, this means the include dir was found, 
#                             as well as all the libraries specified in the COMPONENTS list
#  Boost_INCLUDE_DIRS         Boost include directories, not cached
#  Boost_INCLUDE_DIR          This is almost the same as above, but this one is cached and may be
#                             modified by advanced users
#  Boost_LIBRARIES            Link these to use the Boost libraries that you specified, not cached
#  Boost_LIBRARY_DIRS         The path to where the Boost library files are.
#  Boost_VERSION              The version number of the boost libraries that have been found, 
#                             same as in version.hpp from Boost
#  Boost_LIB_VERSION          The version number in filename form as its appended to the library filenames
#
# These are only set when the named libraries are found and where listed in the COMPONENTS list:
#
#  Boost_DATE_TIME_FOUND           True IF Boost Date Time was found.
#  Boost_FILESYSTEM_FOUND          True IF Boost Filesystem was found.
#  Boost_IOSTREAMS_FOUND           True IF Boost Iostream was found.
#  Boost_PRG_EXEC_MONITOR_FOUND    True IF Boost Program Exec Monitor was found.
#  Boost_PROGRAM_OPTIONS_FOUND     True IF Boost Program Options was found.
#  Boost_PYTHON_FOUND              True IF Boost Python was found.
#  Boost_REGEX_FOUND               True IF Boost Regex was found.
#  Boost_SERIALIZATION_FOUND       True IF Boost Serialization was found.
#  Boost_SIGNALS_FOUND             True IF Boost Signals was found.
#  Boost_TEST_EXEC_MONITOR_FOUND   True IF Boost Test Exec Monitor was found.
#  Boost_THREAD-MT_FOUND           True IF Boost Thread was found.
#  Boost_UNIT_TEST_FRAMEWORK_FOUND True IF Boost Unit Test Framework was found.
#  Boost_WSERIALIZATION_FOUND      True IF Boost WSerialization was found.
#
#  Boost_DATE_TIME_LIBRARY                    The Boost date_time libraries
#  Boost_DATE_TIME_LIBRARY_DEBUG              The Boost date_time debug library
#  Boost_DATE_TIME_LIBRARY_RELEASE            The Boost date_time release library
#
#  Boost_FILESYSTEM_LIBRARY                   The Boost filesystem libraries
#  Boost_FILESYSTEM_LIBRARY_DEBUG             The Boost filesystem debug library
#  Boost_FILESYSTEM_LIBRARY_RELEASE           The Boost filesystem release library
#
#  Boost_IOSTREAMS_LIBRARY                    The Boost iostreams libraries
#  Boost_IOSTREAMS_LIBRARY_DEBUG              The Boost iostreams debug library
#  Boost_IOSTREAMS_LIBRARY_RELEASE            The Boost iostreams release library
#
#  Boost_PRO_EXEC_MONITOR_LIBRARY             The Boost prg_exec_monitor libraries
#  Boost_PRO_EXEC_MONITOR_LIBRARY_DEBUG       The Boost prg_exec_monitor debug library
#  Boost_PRO_EXEC_MONITOR_LIBRARY_RELEASE     The Boost prg_exec_monitor release library
#
#  Boost_PROGRAM_OPTIONS_LIBRARY              The Boost program_options libraries
#  Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG        The Boost program_options debug library
#  Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE      The Boost program_options release library
#
#  Boost_PYTHON_LIBRARY                       The Boost python libraries
#  Boost_PYTHON_LIBRARY_DEBUG                 The Boost python debug library
#  Boost_PYTHON_LIBRARY_RELEASE               The Boost python release library
#
#  Boost_REGEX_LIBRARY                        The Boost regex libraries
#  Boost_REGEX_LIBRARY_DEBUG                  The Boost regex debug library
#  Boost_REGEX_LIBRARY_RELEASE                The Boost regex release library
#
#  Boost_SERIALIZATION_LIBRARY                The Boost serialization libraries
#  Boost_SERIALIZATION_LIBRARY_DEBUG          The Boost serialization debug library
#  Boost_SERIALIZATION_LIBRARY_RELEASE        The Boost serialization release library
#
#  Boost_SIGNALS_LIBRARY                      The Boost signals libraries
#  Boost_SIGNALS_LIBRARY_DEBUG                The Boost signals debug library
#  Boost_SIGNALS_LIBRARY_RELEASE              The Boost signals release library
#
#  Boost_TEST_EXEC_MONITOR_LIBRARY            The Boost test_exec_monitor libraries
#  Boost_TEST_EXEC_MONITOR_LIBRARY_DEBUG      The Boost test_exec_monitor debug library
#  Boost_TEST_EXEC_MONITOR_LIBRARY_RELEASE    The Boost test_exec_monitor release library
#
#  Boost_THREAD_LIBRARY                       The Boost thread libraries
#  Boost_THREAD_LIBRARY_DEBUG                 The Boost thread debug library
#  Boost_THREAD_LIBRARY_RELEASE               The Boost thread release library
#
#  Boost_UNIT_TEST_FRAMEWORK_LIBRARY          The Boost unit_test_framework libraries
#  Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG    The Boost unit_test_framework debug library
#  Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE  The Boost unit_test_framework release library
#
#  Boost_WSERIALIZATION_LIBRARY               The Boost wserialization libraries
#  Boost_WSERIALIZATION_LIBRARY_DEBUG         The Boost wserialization debug library
#  Boost_WSERIALIZATION_LIBRARY_RELEASE       The Boost wserialization release library
#
#  Copyright (c) 2006 Andreas Schneider <mail@cynapses.org>
#  Copyright (c) 2007 Wengo
#  Copyright (c) 2007 Andreas Pakulat <apaku@gmx.de>
#
#  Redistribution AND use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

# MESSAGE(STATUS "Finding Boost libraries.... ")
IF (NOT DEFINED Boost_REQUIRED_VERSION)
    SET ( Boost_REQUIRED_VERSION "1.34.1")
ENDIF (NOT DEFINED Boost_REQUIRED_VERSION)

############################################
#
# Check the existence of the libraries.
#
############################################
# This macro was taken directly from the FindQt4.cmake file that is included
# with the CMake distribution. This is NOT my work. All work was done by the
# original authors of the FindQt4.cmake file. Only minor modifications were
# made to remove references to Qt and make this file more generally applicable
#########################################################################

MACRO (_Boost_ADJUST_LIB_VARS basename)
  IF (Boost_INCLUDE_DIR )
    #MESSAGE(STATUS "Adjusting ${basename} ")

    IF (Boost_${basename}_LIBRARY_DEBUG AND Boost_${basename}_LIBRARY_RELEASE)
      # if the generator supports configuration types then set
      # optimized and debug libraries, or if the CMAKE_BUILD_TYPE has a value
      IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        SET(Boost_${basename}_LIBRARY optimized ${Boost_${basename}_LIBRARY_RELEASE} debug ${Boost_${basename}_LIBRARY_DEBUG})
      ELSE(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        # if there are no configuration types and CMAKE_BUILD_TYPE has no value
        # then just use the release libraries
        SET(Boost_${basename}_LIBRARY ${Boost_${basename}_LIBRARY_RELEASE} )
      ENDIF(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
      SET(Boost_${basename}_LIBRARIES optimized ${Boost_${basename}_LIBRARY_RELEASE} debug ${Boost_${basename}_LIBRARY_DEBUG})
    ENDIF (Boost_${basename}_LIBRARY_DEBUG AND Boost_${basename}_LIBRARY_RELEASE)

    # if only the release version was found, set the debug variable also to the release version
    IF (Boost_${basename}_LIBRARY_RELEASE AND NOT Boost_${basename}_LIBRARY_DEBUG)
      SET(Boost_${basename}_LIBRARY_DEBUG ${Boost_${basename}_LIBRARY_RELEASE})
      SET(Boost_${basename}_LIBRARY       ${Boost_${basename}_LIBRARY_RELEASE})
      SET(Boost_${basename}_LIBRARIES     ${Boost_${basename}_LIBRARY_RELEASE})
    ENDIF (Boost_${basename}_LIBRARY_RELEASE AND NOT Boost_${basename}_LIBRARY_DEBUG)
    
    # if only the debug version was found, set the release variable also to the debug version
    IF (Boost_${basename}_LIBRARY_DEBUG AND NOT Boost_${basename}_LIBRARY_RELEASE)
      SET(Boost_${basename}_LIBRARY_RELEASE ${Boost_${basename}_LIBRARY_DEBUG})
      SET(Boost_${basename}_LIBRARY         ${Boost_${basename}_LIBRARY_DEBUG})
      SET(Boost_${basename}_LIBRARIES       ${Boost_${basename}_LIBRARY_DEBUG})
    ENDIF (Boost_${basename}_LIBRARY_DEBUG AND NOT Boost_${basename}_LIBRARY_RELEASE)

    IF (Boost_${basename}_LIBRARY)
      SET(Boost_${basename}_LIBRARY ${Boost_${basename}_LIBRARY} CACHE FILEPATH "The Boost ${basename} library")
      GET_FILENAME_COMPONENT(Boost_LIBRARY_DIRS ${Boost_${basename}_LIBRARY} PATH)
      SET(Boost_${basename}_FOUND 1)
    ENDIF (Boost_${basename}_LIBRARY)

  ENDIF (Boost_INCLUDE_DIR )
  # Make variables changeble to the advanced user
  MARK_AS_ADVANCED(
      Boost_${basename}_LIBRARY 
      Boost_${basename}_LIBRARY_RELEASE 
      Boost_${basename}_LIBRARY_DEBUG 
  )
ENDMACRO (_Boost_ADJUST_LIB_VARS)

#-------------------------------------------------------------------------------


SET( _boost_IN_CACHE TRUE)

IF(Boost_INCLUDE_DIR)
  FOREACH(COMPONENT ${Boost_FIND_COMPONENTS})
    STRING(TOUPPER ${COMPONENT} COMPONENT)
    IF(NOT Boost_${COMPONENT}_FOUND)
      SET( _boost_IN_CACHE FALSE)
    ENDIF(NOT Boost_${COMPONENT}_FOUND)
  ENDFOREACH(COMPONENT)
ELSE(Boost_INCLUDE_DIR)
  SET( _boost_IN_CACHE FALSE)
ENDIF(Boost_INCLUDE_DIR)

IF (_boost_IN_CACHE)
  # in cache already
  SET(Boost_FOUND TRUE)
  FOREACH(COMPONENT ${Boost_FIND_COMPONENTS})
    STRING(TOUPPER ${COMPONENT} COMPONENT)
    _Boost_ADJUST_LIB_VARS( ${COMPONENT} )
  FOREACH(COMPONENT)
  SET(Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIR})
ELSE (_boost_IN_CACHE)
  # Need to search for boost



  # Add in a path suffix, based on the required version, ideally we could
  # read this from version.hpp, but for that to work we'd need to know the include
  # dir already
  SET(_boost_PATH_SUFFIX
    boost-${Boost_REQUIRED_VERSION}
  )
  STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1_\\2_\\3" _boost_PATH_SUFFIX ${_boost_PATH_SUFFIX})

  SET(Boost_INCLUDE_SEARCH_DIRS
    $ENV{BoostINCLUDEDIR}
    $ENV{Boost_ROOT}/include
    C:/boost/include
    "C:/Program Files/boost/boost_${Boost_REQUIRED_VERSION}"
    # D: is very often the cdrom drive, IF you don't have a
    # cdrom inserted it will popup a very annoying dialog
    #D:/boost/include
    /opt/local/include
  )
  
  SET(_boost_LIBRARIES_SEARCH_DIRS
    $ENV{BoostLIBDIR}
    $ENV{Boost_ROOT}/lib
    C:/boost/lib
    "C:/Program Files/boost/boost_${Boost_REQUIRED_VERSION}/lib"
    /opt/local/lib
  )

  FIND_PATH(Boost_INCLUDE_DIR
    NAMES
      boost/config.hpp
    PATHS
      ${_boost_INCLUDE_SEARCH_DIRS}
    PATH_SUFFIXES
      ${_boost_PATH_SUFFIX}
  )

  # Extract Boost_VERSION and Boost_LIB_VERSION from version.hpp
  # Read the whole file:
  #
  SET(BOOST_VERSION 0)
  SET(BOOST_LIB_VERSION "")
  FILE(READ "${Boost_INCLUDE_DIR}/boost/version.hpp" _boost_VERSION_HPP_CONTENTS)

  STRING(REGEX REPLACE ".*#define BOOST_VERSION ([0-9]+).*" "\\1" Boost_VERSION ${_boost_VERSION_HPP_CONTENTS})
  STRING(REGEX REPLACE ".*#define BOOST_LIB_VERSION \"([0-9_]+)\".*" "\\1" Boost_LIB_VERSION ${_boost_VERSION_HPP_CONTENTS})

  SET(Boost_LIB_VERSION ${Boost_LIB_VERSION} CACHE STRING "The library version string for boost libraries")
  SET(Boost_VERSION ${Boost_VERSION} CACHE STRING "The version number for boost libraries")

  #Setting some more suffixes for the library
  SET (Boost_LIB_PREFIX "")
  IF ( WIN32 )
    SET (Boost_LIB_PREFIX "lib")
  ENDIF ( WIN32 )
  SET (_boost_COMPILER "-gcc")
  IF (MSVC71)
    SET (_boost_COMPILER "-vc71")
  ENDIF(MSVC71)
   IF (MSVC80)
    SET (_boost_COMPILER "-vc80")
  ENDIF(MSVC80) 
  IF (MINGW)
    SET (_boost_COMPILER "-mgw")
  ENDIF(MINGW)
  IF (CYGWIN)
    SET (_boost_COMPILER "-gcc")
  ENDIF (CYGWIN)
  IF (UNIX)
    IF (APPLE)
        SET (_boost_COMPILER "")
    ELSE (APPLE)
      IF (NOT CMAKE_COMPILER_IS_GNUCC)
        # This is for the intel compiler
        SET (_boost_COMPILER "-il")
      ELSE (NOT CMAKE_COMPILER_IS_GNUCC)
        #find out the version of gcc being used.
        EXEC_PROGRAM(${CMAKE_CXX_COMPILER}
            ARGS --version
            OUTPUT_VARIABLE _boost_COMPILER_VERSION
        )
        STRING(REGEX REPLACE ".* ([0-9])\\.([0-9])\\.[0-9] .*" "\\1\\2" 
		_boost_COMPILER_VERSION ${_boost_COMPILER_VERSION})
        SET (_boost_COMPILER "-gcc${_boost_COMPILER_VERSION}")
      ENDIF (NOT CMAKE_COMPILER_IS_GNUCC)
    ENDIF (APPLE)
  ENDIF(UNIX)
  
  SET (_boost_MULTITHREADED "-mt")
 
  IF( Boost_USE_NONMULTITHREADED )
    SET (_boost_MULTITHREADED "")
  ENDIF( Boost_USE_NONMULTITHREADED )
  

  IF (WIN32)
    SET (Boost_ABI_TAG "g")
  ENDIF(WIN32)
  SET (Boost_ABI_TAG "-${Boost_ABI_TAG}d")

  # ------------------------------------------------------------------------
  #  Begin finding boost libraries
  # ------------------------------------------------------------------------
  FOREACH(COMPONENT ${Boost_FIND_COMPONENTS})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)
    FIND_LIBRARY(Boost_${UPPERCOMPONENT}_LIBRARY_RELEASE 
        NAMES ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_COMPILER}${_boost_MULTITHREADED}-${Boost_LIB_VERSION}
	PATHS ${_boost_LIBRARIES_SEARCH_DIRS}
    )

    FIND_LIBRARY(Boost_${UPPERCOMPONENT}_LIBRARY_DEBUG
        NAMES ${Boost_LIB_PREFIX}boost_${COMPONENT}${_boost_COMPILER}${_boost_MULTITHREADED}${Boost_ABI_TAG}-${Boost_LIB_VERSION}
	PATHS ${_boost_LIBRARIES_SEARCH_DIRS}
    )
    _Boost_ADJUST_LIB_VARS(${UPPERCOMPONENT})
  ENDFOREACH(COMPONENT)
  # ------------------------------------------------------------------------
  #  End finding boost libraries
  # ------------------------------------------------------------------------

  SET(Boost_INCLUDE_DIRS
    ${Boost_INCLUDE_DIR}
  )
  
  # MESSAGE(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
  # MESSAGE(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
 
  SET(Boost_FOUND FALSE)
  IF(Boost_INCLUDE_DIR)
    SET( Boost_FOUND TRUE )
    FOREACH(COMPONENT ${Boost_FIND_COMPONENTS})
      STRING(TOUPPER ${COMPONENT} COMPONENT)
      IF(NOT Boost_${COMPONENT}_FOUND)
        SET( Boost_FOUND FALSE)
      ENDIF(NOT Boost_${COMPONENT}_FOUND)
    ENDFOREACH(COMPONENT)
  ELSE(Boost_INCLUDE_DIR)
    SET( Boost_FOUND FALSE)
  ENDIF(Boost_INCLUDE_DIR)
  
  IF (Boost_FOUND)
      IF (NOT Boost_FIND_QUIETLY)
        MESSAGE(STATUS "Found The Following Boost Libraries:")
        FOREACH ( COMPONENT  ${Boost_FIND_COMPONENTS} )
          STRING( TOUPPER ${COMPONENT} ${UPPERCOMPONENT} )
	  IF ( Boost_${UPPERCOMPONENT}_FOUND )
            MESSAGE (STATUS "  ${TMP_LIB}")
	  ENDIF ( Boost_${UPPERCOMPONENT}_FOUND )
        ENDFOREACH(COMPONENT)
      ENDIF(NOT Boost_FIND_QUIETLY)
  ELSE (Boost_FOUND)
      IF (Boost_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Please install the Boost libraries AND development packages")
      ENDIF(Boost_FIND_REQUIRED)
  ENDIF(Boost_FOUND)
  
  # Under Windows, automatic linking is performed, so no need to specify the libraries.
  IF (WIN32)
      SET(Boost_LIBRARIES "")
  ENDIF(WIN32)
  
  # show the Boost_INCLUDE_DIRS AND Boost_LIBRARIES variables only in the advanced view
  MARK_AS_ADVANCED(Boost_INCLUDE_DIRS 
      Boost_LIBRARIES 
      Boost_LIBRARY_DIRS 
  )

ENDIF(_boost_IN_CACHE)
