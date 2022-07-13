############################################################
# Basic
############################################################
cmake_minimum_required(VERSION 3.16.3)

# project name and version
project(Template VERSION 1.0)

# list of all created source files
set(SOURCES
    Src/SimpleClass.cpp
    Src/Main.cpp
)

# list of all created header files
set(HEADERS
    Include/Config.h.in
    Include/SimpleClass.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

# adding config file
configure_file(Include/Config.h.in Include/Config.h)

############################################################
# Git modules
############################################################
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()
endif()

############################################################
# External Libraries
############################################################
option(ADD_GLFW    "Include glfw lib in the project"    OFF)
option(ADD_NCURSES "Include ncurses lib in the project" OFF)
option(ADD_BOOST   "Include boost lib in the project"   OFF)
option(ADD_OPENCV  "Include opencv lib in the project"  OFF)
option(ADD_FTXUI   "Include FTXUI lib in the project"   OFF)
option(ADD_MYSQL   "Include MysQL lib in the project"   OFF)
option(ADD_CURL    "Include curl lib in a project"      OFF)
option(ADD_POCO    "Include poco lib in a project"      OFF)

# GLFW
if(ADD_GLFW)
    add_subdirectory(Lib/glfw)

    list(APPEND EXTRA_LIBS     "${PROJECT_SOURCE_DIR}/Lib/glfw/src")
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/Lib/glfw/include")
    list(APPEND EXTRA_LINKS    glfw)
endif()

# CURSES
if(ADD_NCURSES)
    find_package(Curses REQUIRED)

    list(APPEND EXTRA_INCLUDES ${CURSES_INCLUDE_DIR})
    list(APPEND EXTRA_LINKS    -lcdk -lmenu -lpanel -lncurses)
endif()

# BOOST
if(ADD_BOOST)
    set(Boost_FIND_REQUIRED TRUE)
    set(Boost_FIND_QUIETLY TRUE)
    set(Boost_DEBUG FALSE)
    set(Boost_USE_MULTITHREADED TRUE)
    set(Boost_USE_STATIC_LIBS TRUE)

    find_package(Boost COMPONENTS regex)

    list(APPEND EXTRA_INCLUDES ${Boost_INCLUDE_DIRS})
    list(APPEND EXTRA_LINKS    ${Boost_LIBRARIES})    
endif()

# OPENCV
if(ADD_OPENCV)
    find_package(OpenCV REQUIRED)

    list(APPEND EXTRA_INCLUDES ${OpenCV_INCLUDE_DIRS})
    list(APPEND EXTRA_LINKS    ${OpenCV_LIBS})
endif()

# FTXUI
if(ADD_FTXUI)
    include(FetchContent)

    FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v3.0.0
    )

    FetchContent_GetProperties(ftxui)
    if(NOT ftxui_POPULATED)
        FetchContent_Populate(ftxui)
        add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()

    list(APPEND EXTRA_LINKS
        ftxui::screen
        ftxui::dom
        ftxui::component
    )
endif()

# MYSQL
if(ADD_MYSQL)
    # paths may differ depending on how you installed mysql libraries
    list(APPEND EXTRA_INCLUDES  "/usr/include/cppconn")
    list(APPEND EXTRA_LIBS      "/usr/lib")    
    list(APPEND EXTRA_LINKS     mysqlcppconn)
endif()

# CURL
if(ADD_CURL)
    find_package(CURL REQUIRED) 
    
    list(APPEND EXTRA_INCLUDES ${CURL_INCLUDE_DIR})
    list(APPEND EXTRA_LINKS    ${CURL_LIBRARIES})
endif()

# POCO
if(ADD_POCO)
    find_package(Poco REQUIRED Foundation Util Net Crypto NetSSL )
    list(APPEND EXTRA_LINKS    Poco::Foundation Poco::Util Poco::Net Poco::Crypto Poco::NetSSL)
endif()

############################################################
# Target 
############################################################
# adding subfolders to the project
add_subdirectory(Lib)
add_subdirectory(Tests)
add_subdirectory(Data)

# adding list of all external libriaries header files
target_include_directories(${PROJECT_NAME} 
    PRIVATE ${PROJECT_BINARY_DIR}
    PRIVATE ${EXTRA_INCLUDES}
    PRIVATE ${PROJECT_SOURCE_DIR}/Include
)

# adding list of all external libriaries source files
target_link_directories(${PROJECT_NAME}
    PRIVATE ${EXTRA_LIBS}
)

# linking all external libraries
target_link_libraries(${PROJECT_NAME} 
    ${EXTRA_LINKS}
)
