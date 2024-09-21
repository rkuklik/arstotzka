cmake_minimum_required(VERSION 3.16)

project(arstotzka)

set(QT_MIN_VERSION "6.6.0")
set(KF_MIN_VERSION "6.6.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs6)
include(KDECMakeSettings)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core)
find_package(KF6 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Config ConfigWidgets)
find_package(KDecoration2 REQUIRED)
find_package(KF6CoreAddons REQUIRED)

add_library(arstotzka MODULE)
target_sources(arstotzka PRIVATE decoration.cpp decoration.hpp)
target_link_libraries(
    arstotzka
    PUBLIC Qt::Core
    PRIVATE KDecoration2::KDecoration KF6::ConfigCore KF6::ConfigWidgets KF6::CoreAddons
)

install(
    TARGETS arstotzka
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/org.kde.kdecoration2
)
