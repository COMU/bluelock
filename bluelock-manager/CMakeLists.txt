cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(bluelock-manager)

set(BLUELOCK_MANAGER_MAJOR_VERSION 0)
set(BLUELOCK_MANAGER_MINOR_VERSION 1)
set(BLUELOCK_MANAGER_PATCH_VERSION 0)
set(BLUELOCK_MANAGER_VERSION ${BLUELOCK_MANAGER_MAJOR_VERSION}.${BLUELOCK_MANAGER_MINOR_VERSION}.${BLUELOCK_MANAGER_PATCH_VERSION})

add_definitions(
    -DAPPLICATION_NAME="project"
    -DAPPLICATION_DISPLAY_NAME="Bluelock Manager"
    -DAPPLICATION_VERSION="${BLUELOCK_MANAGER_VERSION}"

    -DORGANIZATION_NAME="kafamerkezi"
    -DORGANIZATION_DOMAIN="kafamerkezi.com"

    -DBT_SCAN_TIME=3000
    -DSTATUS_MESSAGE_TIME=3000
    -DCONFIG_PATH="/etc/bluelock.conf"
    )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Bluetooth REQUIRED)
find_package(Qt5DBus REQUIRED)

set(qt_sources
    main.cpp
    mainwindow.cpp
    aboutdialog.cpp
    customwidgets/bluetoothitemwidget.cpp
    bluetoothitem.cpp
    blueconfpp/blueconfpp.cpp
)
set(qt_headers
    mainwindow.h
    aboutdialog.h
    customwidgets/bluetoothitemwidget.h
    bluetoothitem.h
    blueconfpp/blueconfpp.h    
)
set(qt_forms
    mainwindow.ui
    aboutdialog.ui
    customwidgets/bluetoothitemwidget.ui
)

qt5_wrap_cpp(
    qt_headers_moc ${qt_headers}
    )

qt5_wrap_ui(
    qt_forms_moc ${qt_forms}
    )

add_executable(bluelock-manager
    ${qt_sources} ${qt_headers} ${qt_forms}
    )

target_link_libraries(bluelock-manager 
    Qt5::Widgets
    Qt5::Bluetooth
    Qt5::DBus
    blueconf
    )
