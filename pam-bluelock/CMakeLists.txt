SET(GCC_COVERAGE_COMPILE_FLAGS "-pthread -fPIC -fno-stack-protector")
SET( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

set(PAM_BLUELOCK_LIBRARIES
    "blueconfs"
    )
set(EXTRA_LIBS 
    -lcrypt
    -lbluetooth
    )

add_library(pam-bluelock SHARED pam_bluelock.c)

target_link_libraries(pam-bluelock "${PAM_BLUELOCK_LIBRARIES}")
target_link_libraries(pam-bluelock "${EXTRA_LIBS}")
