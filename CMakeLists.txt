cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(bluelock)
 
set(BLUELOCK_MAJOR_VERSION 0)
set(BLUELOCK_MINOR_VERSION 1)
set(BLUELOCK_PATCH_VERSION 0)
set(BLUELOCK_VERSION
  ${BLUELOCK_MAJOR_VERSION}.${BLUELOCK_MINOR_VERSION}.${BLUELOCK_PATCH_VERSION})

add_definitions(
    -DCONFIG_PATH="/etc/bluelock.conf"
    )

include_directories(
    blueconf/
    ${CMAKE_CURRENT_BINARY_DIR}/blueconf
    )

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

add_subdirectory(blueconf)
add_subdirectory(pam-bluelock)
add_subdirectory(bluelock-manager)
