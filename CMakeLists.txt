cmake_minimum_required(VERSION 3.10)

set(PROJECT_TITLE native-lib-test)

project(${PROJECT_TITLE} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(SFML_DIR "C:/Users/Dilorfin/AppData/Local/Android/Sdk/ndk/23.1.7779620/sources/third_party/sfml/lib/armeabi-v7a")
set(SFML_INCLUDE_DIR "${SFML_DIR}/../../include")

message(${SFML_INCLUDE_DIR})

add_library(SFML::Main STATIC IMPORTED)
set_target_properties(SFML::Main PROPERTIES IMPORTED_LOCATION "${SFML_DIR}/libsfml-main-d.a")

add_library(SFML::Activity SHARED IMPORTED)
set_target_properties(SFML::Activity PROPERTIES IMPORTED_LOCATION "${SFML_DIR}/libsfml-activity-d.so")

add_library(SFML::System SHARED IMPORTED)
set_target_properties(SFML::System PROPERTIES IMPORTED_LOCATION "${SFML_DIR}/libsfml-system-d.so")

add_library(SFML::Window SHARED IMPORTED)
set_target_properties(SFML::Window PROPERTIES IMPORTED_LOCATION "${SFML_DIR}/libsfml-window-d.so")

add_library(SFML::Graphics SHARED IMPORTED)
set_target_properties(SFML::Graphics PROPERTIES IMPORTED_LOCATION "${SFML_DIR}/libsfml-graphics-d.so")

add_library(${PROJECT_TITLE} SHARED source/main.cpp)

target_include_directories(${PROJECT_TITLE} PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_TITLE} SFML::Main SFML::Activity SFML::System SFML::Window SFML::Graphics)
