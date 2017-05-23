cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(blueconf)

set(BLUECONF_MAJOR_VERSION 0)
set(BLUECONF_MINOR_VERSION 1)
set(BLUECONF_PATCH_VERSION 0)
set(BLUECONF_VERSION ${BLUECONF_MAJOR_VERSION}.${BLUECONF_MINOR_VERSION}.${BLUECONF_PATCH_VERSION})

add_definitions(
    -DCONFIG_PATH="/etc/bluelock.conf"
    )

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)

BISON_TARGET(blueparse blueparse.y ${CMAKE_CURRENT_BINARY_DIR}/blueparse.tab.c)
FLEX_TARGET(bluelex bluelex.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
ADD_FLEX_BISON_DEPENDENCY(bluelex blueparse)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )
ADD_LIBRARY(blueconf SHARED blueconf.c ${BISON_blueparse_OUTPUTS} ${FLEX_bluelex_OUTPUTS})

SET(GCC_COVERAGE_COMPILE_FLAGS "-fPIC")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

ADD_LIBRARY(blueconfs STATIC blueconf.c ${BISON_blueparse_OUTPUTS} ${FLEX_bluelex_OUTPUTS})


