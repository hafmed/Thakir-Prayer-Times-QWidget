cmake_minimum_required(VERSION 3.16)

project(thakir_prayer_times VERSION 0.1 LANGUAGES CXX C)

set(APP_VERSION 2.0.9)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(src/version.h.in src/version.h)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Multimedia DBus)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Multimedia)

qt_standard_project_setup(REQUIRES 6.5)

set(CMAKE_AUTORCC ON)

set(ANDROID_PACKAGE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/android CACHE INTERNAL "")

set(HEADERS
    src/about.h
    src/prayer.h
    src/engine.h
    src/thakir_prayer_times.h
    src/version.h.in
    src/constants.h
)

set(SOURCES
    src/main.cpp
    src/prayer.c
    src/thakir_prayer_times.cpp
    src/about.cpp
    src/astro.c
    src/prayer.c
    src/engine.cpp
)

set(FORMS
    src/about.ui
    src/thakir_prayer_times.ui
)

qt_add_executable(thakir_prayer_times
    ${HEADERS}
    ${SOURCES}
    ${FORMS}
    qtresource.qrc winlogohaf.rc
    io.github.hafmed.Thakir-Prayer-Times.png
    io.github.hafmed.Thakir-Prayer-Times.svg
    io.github.hafmed.Thakir-Prayer-Times
    io.github.hafmed.Thakir-Prayer-Times.yaml
    io.github.hafmed.Thakir-Prayer-Times.metainfo.xml
    io.github.hafmed.Thakir-Prayer-Times.desktop
)

target_link_libraries(thakir_prayer_times PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Core Qt6::Multimedia Qt6::DBus)

set_target_properties(thakir_prayer_times PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS thakir_prayer_times
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(thakir_prayer_times)
endif()

# install(FILES ${PROJECT_SOURCE_DIR}/io.github.hafmed.Thakir-Prayer-Times.metainfo.xml DESTINATION $ENV{HOME}/.local/app/share/metainfo/io.github.hafmed.Thakir-Prayer-Times.metainfo.xml)

install(FILES "${PROJECT_SOURCE_DIR}/io.github.hafmed.Thakir-Prayer-Times.metainfo.xml"
        DESTINATION "$ENV{HOME}/.local/app/share/metainfo")
install(FILES "${PROJECT_SOURCE_DIR}/io.github.hafmed.Thakir-Prayer-Times.desktop"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/applications")
install(FILES "${PROJECT_SOURCE_DIR}/io.github.hafmed.Thakir-Prayer-Times.metainfo.xml"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/metainfo")
install(FILES "${PROJECT_SOURCE_DIR}/io.github.hafmed.Thakir-Prayer-Times.png"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/256x256/apps")

        
