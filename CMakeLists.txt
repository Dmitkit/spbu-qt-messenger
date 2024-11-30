cmake_minimum_required(VERSION 3.5)

<<<<<<< HEAD
project(Mesger VERSION 0.1 LANGUAGES CXX)
=======
project(spbu-qt-messenger VERSION 0.1 LANGUAGES CXX)
>>>>>>> fd4b0ca (Added users, dialogs)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

<<<<<<< HEAD
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
=======
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Quick Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Quick Sql Network)
find_package(Qt6 REQUIRED COMPONENTS Network Quick Sql)

>>>>>>> fd4b0ca (Added users, dialogs)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
<<<<<<< HEAD
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Mesger
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        chatwindow.h chatwindow.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Mesger APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
=======
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(spbu-qt-messenger
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        chatwindow.h chatwindow.cpp
        chatwindow.h chatwindow.cpp
        chatwindow.ui
        server.h server.cpp
        resources.qrc
        database/database.cpp database/database.h
        client/client.cpp client/client.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET spbu-qt-messenger APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
>>>>>>> fd4b0ca (Added users, dialogs)
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
<<<<<<< HEAD
        add_library(Mesger SHARED
=======
        add_library(spbu-qt-messenger SHARED
>>>>>>> fd4b0ca (Added users, dialogs)
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
<<<<<<< HEAD
        add_executable(Mesger
            ${PROJECT_SOURCES}
=======
        add_executable(spbu-qt-messenger
            ${PROJECT_SOURCES}
            resources.qrc
>>>>>>> fd4b0ca (Added users, dialogs)
        )
    endif()
endif()

<<<<<<< HEAD
target_link_libraries(Mesger PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
=======
target_link_libraries(spbu-qt-messenger PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(spbu-qt-messenger PRIVATE Qt6::Network)
target_link_libraries(spbu-qt-messenger PRIVATE Qt6::Sql)

>>>>>>> fd4b0ca (Added users, dialogs)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
<<<<<<< HEAD
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Mesger)
endif()
set_target_properties(Mesger PROPERTIES
=======
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.spbu-qt-messenger)
endif()
set_target_properties(spbu-qt-messenger PROPERTIES
>>>>>>> fd4b0ca (Added users, dialogs)
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
<<<<<<< HEAD
install(TARGETS Mesger
=======
install(TARGETS spbu-qt-messenger
>>>>>>> fd4b0ca (Added users, dialogs)
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
<<<<<<< HEAD
    qt_finalize_executable(Mesger)
=======
    qt_finalize_executable(spbu-qt-messenger)
>>>>>>> fd4b0ca (Added users, dialogs)
endif()
