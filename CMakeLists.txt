cmake_minimum_required(VERSION 3.16)

project(arstotzka)

set(QT_VER "6.8.0")
set(KF_VER "6.3.0")
set(QT_MAJOR_VERSION 6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ECM ${KF_VER} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs6)
include(KDECMakeSettings)

find_package(Qt6 ${QT_VER} CONFIG REQUIRED COMPONENTS Core)
find_package(KDecoration3 ${KF_VER} REQUIRED)
find_package(KF6CoreAddons ${KF_VER} REQUIRED)
find_package(KF6Config ${KF_VER} REQUIRED)
find_package(KF6ConfigWidgets ${KF_VER} REQUIRED)

add_library(arstotzka MODULE)
target_sources(arstotzka PRIVATE decoration.cpp decoration.hpp)
target_link_libraries(
    arstotzka
    PUBLIC Qt::Core
    PRIVATE KDecoration3::KDecoration KF6::ConfigCore KF6::ConfigWidgets KF6::CoreAddons
)

install(
    TARGETS arstotzka
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/org.kde.kdecoration3
)
