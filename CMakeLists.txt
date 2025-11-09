cmake_minimum_required(VERSION 3.16)

project(mychat VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "D:/Qt/Qt6.5/6.5.3/mingw_64")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network)


set(HEADER_SOURCES
    const.h
    mainwindow.h
    login_dialog.h
    register_dialog.h
    timer_btn.h
    user_data.h
    user_mgr.h
    chat_window.h
    chat_user_list.h
    chat_user_item.h
    chat_widget.h
    chat_message_edit_widget.h
    chat_message_show_widget.h
    message_item.h
    # self_message_widget.h
    # other_message_widget.h
    message_text_edit.h
    apply_widget.h
    apply_friend_list.h
    search_list.h
    search_user_item.h
    apply_friend_dialog.h
    friend_list.h
    friend_item.h
)

set(NET_SOURCES
    net/http_mgr.h
    net/http_mgr.cpp
    net/tcp_mgr.h
    net/tcp_mgr.cpp
)

set(UTILS_SOURCES
    utils/singleton.h
    utils/util.h
    utils/util.cpp
    utils/clicked_label.h
    utils/clicked_label.cpp
    utils/clicked_btn.h
    utils/clicked_btn.cpp
    utils/customize_edit.h
    utils/customize_edit.cpp
    utils/list_item_base.h
    utils/list_item_base.cpp
    utils/state_widget.h
    utils/state_widget.cpp
)

set(SOURCE_SOURCES
    mainwindow.cpp
    login_dialog.cpp
    register_dialog.cpp
    timer_btn.cpp
    user_mgr.cpp
    chat_window.cpp
    chat_user_list.cpp
    chat_user_item.cpp
    chat_widget.cpp
    chat_message_edit_widget.cpp
    chat_message_show_widget.cpp
    message_item.cpp
    # self_message_widget.cpp
    # other_message_widget.cpp
    message_text_edit.cpp
    apply_widget.cpp
    apply_friend_list.cpp
    search_list.cpp
    search_user_item.cpp
    apply_friend_dialog.cpp
    friend_list.cpp
    friend_item.cpp
    group_tip_item.h
    group_tip_item.cpp
    apply_friend_widget.h
    apply_friend_widget.cpp
    apply_friend_item.h
    apply_friend_item.cpp
    auth_friend_dialog.h
    auth_friend_dialog.cpp
    chat_item_base.h
    chat_item_base.cpp
    user_data.cpp
)

# 聊天气泡
set(UI_SOURCES
    UI/bubble/bubble_frame.h
    UI/bubble/bubble_frame.cpp
    UI/bubble/text_bubble.h
    UI/bubble/text_bubble.cpp
    UI/bubble/picture_bubble.h
    UI/bubble/picture_bubble.cpp
)



set(PROJECT_SOURCES
    main.cpp
    ${UTILS_SOURCES}
    ${NET_SOURCES}
    ${SOURCE_SOURCES}
    ${HEADER_SOURCES}
    ${UI_SOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mychat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET mychat APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(mychat SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(mychat
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(mychat PRIVATE
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Network
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.mychat)
endif()
set_target_properties(mychat PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS mychat
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mychat)
endif()
