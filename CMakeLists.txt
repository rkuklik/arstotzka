cmake_minimum_required(VERSION 3.16)

project(arstotzka)

set(KFQT 5)

if(KFQT EQUAL 5)
    set(QT_MIN_VERSION "5.15.0")
    set(KF_MIN_VERSION "5.78.0")
    set(QT Qt5)
    set(KF KF5)
elseif(KFQT EQUAL 6)
    set(QT_MIN_VERSION "6.4.0")
    set(KF_MIN_VERSION "5.240.0")
    set(QT Qt6)
    set(KF KF6)
endif()

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

include(KDEInstallDirs)
include(KDECMakeSettings)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(${QT} ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core)
find_package(${KF} ${KF5_MIN_VERSION} REQUIRED COMPONENTS Config ConfigWidgets)
find_package(KDecoration2 REQUIRED)

add_library(arstotzka MODULE)
target_sources(arstotzka PRIVATE decoration.cpp decoration.hpp)
target_link_libraries(
    arstotzka
    PUBLIC Qt::Core
    PRIVATE KDecoration2::KDecoration ${KF}::ConfigCore ${KF}::ConfigWidgets
)

install(
    TARGETS arstotzka
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/org.kde.kdecoration2
)
