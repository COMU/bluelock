cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(pam_bluelock)

set(PAM_BLUELOCK_MAJOR_VERSION 0)
set(PAM_BLUELOCK_MINOR_VERSION 1)
set(PAM_BLUELOCK_PATCH_VERSION 0)
set(PAM_BLUELOCK_VERSION ${PAM_BLUELOCK_MAJOR_VERSION}.${PAM_BLUELOCK_MINOR_VERSION}.${PAM_BLUELOCK_PATCH_VERSION})

add_definitions(
    -DCONFIG_PATH="/etc/bluelock.conf"
    )

SET(GCC_COVERAGE_COMPILE_FLAGS "-pthread -fPIC -fno-stack-protector")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(PAM_BLUELOCK_LIBRARIES
    "blueconfs"
    )
set(EXTRA_LIBS 
    -lbluetooth
    )

add_library(pam_bluelock SHARED pam_bluelock.c)

target_link_libraries(pam_bluelock "${PAM_BLUELOCK_LIBRARIES}")
target_link_libraries(pam_bluelock "${EXTRA_LIBS}")
