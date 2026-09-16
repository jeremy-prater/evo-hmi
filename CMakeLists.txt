cmake_minimum_required(VERSION 3.21)
project(evo-hmi VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick Network)
find_package(Qt6 COMPONENTS DBus QuickControls2 Svg QUIET)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(LibOsmScout QUIET)

qt_standard_project_setup(REQUIRES 6.5)
qt_policy(SET QTP0004 NEW)

set(EVO_HMI_SOURCES
    src/main.cpp
    src/ecu/EcuClient.cpp
    src/ecu/FakeEcuClient.cpp
    src/nav/GpsdClient.cpp
    src/nav/ManeuverModel.cpp
    src/nav/NavEngine.cpp
    src/nav/SimulatedNavEngine.cpp
    src/bt/DeviceModel.cpp
    src/bt/BluezController.cpp
)

set(EVO_HMI_QML
    qml/Main.qml
    qml/Theme.qml
    qml/Fa.qml
    qml/Icon.qml
    qml/StatusBar.qml
    qml/Splash.qml
    qml/Dashboard.qml
    qml/Engine.qml
    qml/Map.qml
    qml/Media.qml
    qml/Settings.qml
)

set_source_files_properties(qml/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)
set_source_files_properties(qml/Fa.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_executable(evo-hmi ${EVO_HMI_SOURCES})

qt_add_qml_module(evo-hmi
    URI EvoHmi
    VERSION 1.0
    QML_FILES ${EVO_HMI_QML}
    RESOURCES
        qml/fonts/fa-solid-900.otf
        qml/fonts/fa-regular-400.otf
        qml/fonts/fa-brands-400.otf
    SOURCES
        src/ecu/EcuClient.h
        src/ecu/FakeEcuClient.h
        src/nav/GpsdClient.h
        src/nav/ManeuverModel.h
        src/nav/NavEngine.h
        src/nav/SimulatedNavEngine.h
        src/bt/DeviceModel.h
        src/bt/BluezController.h
        src/nav/ScoutMapItem.h
        src/nav/ScoutMapItem.cpp
)

target_include_directories(evo-hmi PRIVATE src src/nav src/ecu src/bt)

target_link_libraries(evo-hmi PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Network
)

if(TARGET Qt6::QuickControls2)
    target_compile_definitions(evo-hmi PRIVATE EVO_HMI_HAS_QUICKCONTROLS)
    target_link_libraries(evo-hmi PRIVATE Qt6::QuickControls2)
endif()
if(TARGET Qt6::Svg)
    target_link_libraries(evo-hmi PRIVATE Qt6::Svg)
endif()

if(TARGET Qt6::DBus)
    target_compile_definitions(evo-hmi PRIVATE EVO_HMI_HAS_DBUS)
    target_link_libraries(evo-hmi PRIVATE Qt6::DBus)
endif()

if(LibOsmScout_FOUND)
    target_compile_definitions(evo-hmi PRIVATE EVO_HMI_HAS_OSMSCOUT)
    target_sources(evo-hmi PRIVATE
        src/nav/OsmScoutNavEngine.cpp
        src/nav/OsmScoutNavEngine.h
    )
    target_include_directories(evo-hmi PRIVATE ${LibOsmScout_INCLUDE_DIRS})
    target_link_libraries(evo-hmi PRIVATE ${LibOsmScout_LIBRARIES})
    message(STATUS "libosmscout enabled: ${LibOsmScout_LIBRARIES}")
else()
    message(STATUS "libosmscout not found; using simulated routing + canvas map")
endif()

install(TARGETS evo-hmi RUNTIME DESTINATION bin)
