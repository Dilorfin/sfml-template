cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0111 NEW)

set(PROJECT_TITLE native-lib-test)

project(${PROJECT_TITLE} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES Debug Release)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)	
endif(CMAKE_CONFIGURATION_TYPES)

option(SFML_ANDROID_PATH "Path to prebuild SFML binaries for Android")
option(BOX2D_ANDROID_PATH "Path to prebuild box2d binaries for Android")

if(NOT DEFINED CMAKE_ANDROID_NDK)
	if(DEFINED ENV{ANDROID_NDK_HOME})
		set(CMAKE_ANDROID_NDK $ENV{ANDROID_NDK_ROOT})
	else()
		message(FATAL_ERROR "ANDROID_NDK_ROOT is not setted and CMAKE_ANDROID_NDK is not defined")
	endif()
endif()

message(STATUS "CMAKE_ANDROID_NDK: \"${CMAKE_ANDROID_NDK}\"")

if(NOT DEFINED CMAKE_ANDROID_STL_TYPE)
	set(CMAKE_ANDROID_STL_TYPE "c++_static")
	message(STATUS "CMAKE_ANDROID_STL_TYPE was not setted, setting to ${CMAKE_ANDROID_STL_TYPE}")
endif()

if(NOT DEFINED CMAKE_ANDROID_ARCH_ABI)
	set(CMAKE_ANDROID_ARCH_ABI "armeabi-v7a")
	message(STATUS "CMAKE_ANDROID_ARCH_ABI was not setted, setting to ${CMAKE_ANDROID_ARCH_ABI}")
endif()

cmake_path(APPEND CMAKE_ANDROID_NDK "sources" "third_party" OUTPUT_VARIABLE ANDROID_NDK_THIRD_PARTY)

cmake_path(APPEND ANDROID_NDK_THIRD_PARTY "box2d" "lib" ${CMAKE_ANDROID_ARCH_ABI} OUTPUT_VARIABLE BOX2D_LIB_DIR)
cmake_path(APPEND ANDROID_NDK_THIRD_PARTY "box2d" "include" OUTPUT_VARIABLE BOX2D_INCLUDE_DIR)

cmake_path(APPEND ANDROID_NDK_THIRD_PARTY "sfml" "lib" ${CMAKE_ANDROID_ARCH_ABI} OUTPUT_VARIABLE SFML_LIB_DIR)
cmake_path(APPEND ANDROID_NDK_THIRD_PARTY "sfml" "extlibs" "lib" ${CMAKE_ANDROID_ARCH_ABI} OUTPUT_VARIABLE SFML_EXTLIB_DIR)
cmake_path(APPEND ANDROID_NDK_THIRD_PARTY "sfml" "include" OUTPUT_VARIABLE SFML_INCLUDE_DIR)

message(STATUS "BOX2D_LIB_DIR: \"${BOX2D_LIB_DIR}\"")
message(STATUS "BOX2D_INCLUDE_DIR: \"${BOX2D_INCLUDE_DIR}\"")

message(STATUS "SFML_LIB_DIR: \"${SFML_LIB_DIR}\"")
message(STATUS "SFML_EXTLIB_DIR: \"${SFML_EXTLIB_DIR}\"")
message(STATUS "SFML_INCLUDE_DIR: \"${SFML_INCLUDE_DIR}\"")

add_library(SFML::Main STATIC IMPORTED)
set_target_properties(SFML::Main PROPERTIES IMPORTED_LOCATION_DEBUG "${SFML_LIB_DIR}/libsfml-main-d.a")
set_target_properties(SFML::Main PROPERTIES IMPORTED_IMPLIB_DEBUG "${SFML_LIB_DIR}/libsfml-main-d.a")
set_target_properties(SFML::Main PROPERTIES IMPORTED_LOCATION_RELEASE "${SFML_LIB_DIR}/libsfml-main.a")
set_target_properties(SFML::Main PROPERTIES IMPORTED_IMPLIB_RELEASE "${SFML_LIB_DIR}/libsfml-main.a")

add_library(SFML::Activity SHARED IMPORTED)
set_target_properties(SFML::Activity PROPERTIES IMPORTED_LOCATION_DEBUG "${SFML_LIB_DIR}/libsfml-activity-d.so")
set_target_properties(SFML::Activity PROPERTIES IMPORTED_IMPLIB_DEBUG "${SFML_LIB_DIR}/libsfml-activity-d.so")
set_target_properties(SFML::Activity PROPERTIES IMPORTED_LOCATION_RELEASE "${SFML_LIB_DIR}/libsfml-activity.so")
set_target_properties(SFML::Activity PROPERTIES IMPORTED_IMPLIB_RELEASE "${SFML_LIB_DIR}/libsfml-activity.so")

add_library(SFML::System SHARED IMPORTED)
set_target_properties(SFML::System PROPERTIES IMPORTED_LOCATION_DEBUG "${SFML_LIB_DIR}/libsfml-system-d.so")
set_target_properties(SFML::System PROPERTIES IMPORTED_IMPLIB_DEBUG "${SFML_LIB_DIR}/libsfml-system-d.so")
set_target_properties(SFML::System PROPERTIES IMPORTED_LOCATION_RELEASE "${SFML_LIB_DIR}/libsfml-system.so")
set_target_properties(SFML::System PROPERTIES IMPORTED_IMPLIB_RELEASE "${SFML_LIB_DIR}/libsfml-system.so")

add_library(SFML::Window SHARED IMPORTED)
set_target_properties(SFML::Window PROPERTIES IMPORTED_LOCATION_DEBUG "${SFML_LIB_DIR}/libsfml-window-d.so")
set_target_properties(SFML::Window PROPERTIES IMPORTED_IMPLIB_DEBUG "${SFML_LIB_DIR}/libsfml-window-d.so")
set_target_properties(SFML::Window PROPERTIES IMPORTED_LOCATION_RELEASE "${SFML_LIB_DIR}/libsfml-window.so")
set_target_properties(SFML::Window PROPERTIES IMPORTED_IMPLIB_RELEASE "${SFML_LIB_DIR}/libsfml-window.so")

add_library(SFML::Graphics SHARED IMPORTED)
set_target_properties(SFML::Graphics PROPERTIES IMPORTED_LOCATION_DEBUG "${SFML_LIB_DIR}/libsfml-graphics-d.so")
set_target_properties(SFML::Graphics PROPERTIES IMPORTED_IMPLIB_DEBUG "${SFML_LIB_DIR}/libsfml-graphics-d.so")
set_target_properties(SFML::Graphics PROPERTIES IMPORTED_LOCATION_RELEASE "${SFML_LIB_DIR}/libsfml-graphics.so")
set_target_properties(SFML::Graphics PROPERTIES IMPORTED_IMPLIB_RELEASE "${SFML_LIB_DIR}/libsfml-graphics.so")

add_library(SFML::Audio SHARED IMPORTED)
set_target_properties(SFML::Audio PROPERTIES IMPORTED_LOCATION_DEBUG "${SFML_LIB_DIR}/libsfml-audio-d.so")
set_target_properties(SFML::Audio PROPERTIES IMPORTED_IMPLIB_DEBUG "${SFML_LIB_DIR}/libsfml-audio-d.so")
set_target_properties(SFML::Audio PROPERTIES IMPORTED_LOCATION_RELEASE "${SFML_LIB_DIR}/libsfml-audio.so")
set_target_properties(SFML::Audio PROPERTIES IMPORTED_IMPLIB_RELEASE "${SFML_LIB_DIR}/libsfml-audio.so")

add_library(openal SHARED IMPORTED)
set_target_properties(openal PROPERTIES IMPORTED_LOCATION_DEBUG "${SFML_EXTLIB_DIR}/libopenal.so")
set_target_properties(openal PROPERTIES IMPORTED_IMPLIB_DEBUG "${SFML_EXTLIB_DIR}/libopenal.so")
set_target_properties(openal PROPERTIES IMPORTED_LOCATION_RELEASE "${SFML_EXTLIB_DIR}/libopenal.so")
set_target_properties(openal PROPERTIES IMPORTED_IMPLIB_RELEASE "${SFML_EXTLIB_DIR}/libopenal.so")

add_library(box2d::box2d SHARED IMPORTED)
set_target_properties(box2d::box2d PROPERTIES IMPORTED_LOCATION_DEBUG "${BOX2D_LIB_DIR}/libbox2d.so")
set_target_properties(box2d::box2d PROPERTIES IMPORTED_IMPLIB_DEBUG "${BOX2D_LIB_DIR}/libbox2d.so")
set_target_properties(box2d::box2d PROPERTIES IMPORTED_LOCATION_RELEASE "${BOX2D_LIB_DIR}/libbox2d.so")
set_target_properties(box2d::box2d PROPERTIES IMPORTED_IMPLIB_RELEASE "${BOX2D_LIB_DIR}/libbox2d.so")

add_library(${PROJECT_TITLE} SHARED source/main.cpp)

target_include_directories(${PROJECT_TITLE} PRIVATE ${SFML_INCLUDE_DIR} ${BOX2D_INCLUDE_DIR})

target_link_libraries(${PROJECT_TITLE}
	android 
	openal
	box2d::box2d 
	SFML::Activity
	SFML::System
	SFML::Window
	SFML::Graphics 
	SFML::Audio
	-Wl,--whole-archive SFML::Main -Wl,--no-whole-archive
)
