# Install script for directory: /Users/dillonl/Projects/gwiz/externals/bamtools/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/bamtools/shared" TYPE FILE FILES "/Users/dillonl/Projects/gwiz/externals/bamtools/src/shared/bamtools_global.h")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/dillonl/Projects/gwiz/xcodebin/externals/bamtools/src/api/cmake_install.cmake")
  include("/Users/dillonl/Projects/gwiz/xcodebin/externals/bamtools/src/third_party/cmake_install.cmake")
  include("/Users/dillonl/Projects/gwiz/xcodebin/externals/bamtools/src/toolkit/cmake_install.cmake")
  include("/Users/dillonl/Projects/gwiz/xcodebin/externals/bamtools/src/utils/cmake_install.cmake")

endif()

