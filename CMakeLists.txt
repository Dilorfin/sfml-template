cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0111 NEW)

set(PROJECT_TITLE native-lib-test)
project(${PROJECT_TITLE} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#set(ANDROID_APPLICATION_ID "org.sfmldev.android")

option(SFML_ANDROID_PATH "Path to prebuild SFML binaries for Android")
option(BOX2D_ANDROID_PATH "Path to prebuild box2d binaries for Android")

if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES Debug Release)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
endif()

if(NOT DEFINED CMAKE_ANDROID_NDK)
	if(DEFINED ENV{ANDROID_NDK_HOME})
		set(CMAKE_ANDROID_NDK $ENV{ANDROID_NDK_ROOT})
	else()
		message(FATAL_ERROR "ANDROID_NDK_ROOT is not setted and CMAKE_ANDROID_NDK is not defined")
	endif()
endif()

if(NOT EXISTS ${CMAKE_ANDROID_NDK})
	message(FATAL_ERROR "CMAKE_ANDROID_NDK: \"${CMAKE_ANDROID_NDK}\" path does not exist")
endif()
message(STATUS "CMAKE_ANDROID_NDK: \"${CMAKE_ANDROID_NDK}\"")

#if(NOT DEFINED CMAKE_ANDROID_STL_TYPE)
#	set(CMAKE_ANDROID_STL_TYPE "c++_static")
#	message(STATUS "CMAKE_ANDROID_STL_TYPE was not setted, setting to ${CMAKE_ANDROID_STL_TYPE}")
#endif()

if(NOT DEFINED CMAKE_ANDROID_ARCH_ABI)
	set(CMAKE_ANDROID_ARCH_ABI "armeabi-v7a")
	message(STATUS "CMAKE_ANDROID_ARCH_ABI: ${CMAKE_ANDROID_ARCH_ABI}")
endif()

if(NOT SFML_ANDROID_PATH)
	cmake_path(APPEND CMAKE_ANDROID_NDK "sources" "third_party" "sfml" OUTPUT_VARIABLE SFML_ANDROID_PATH)
endif()
if(NOT EXISTS ${SFML_ANDROID_PATH})
	message(FATAL_ERROR "SFML_ANDROID_PATH: \"${SFML_ANDROID_PATH}\" path does not exist")
endif()
if(NOT EXISTS ${SFML_ANDROID_PATH}/Android.mk)
	message(FATAL_ERROR "SFML Android.mk: \"${SFML_ANDROID_PATH}/Android.mk\" does not exist")
endif()
message(STATUS "SFML_ANDROID_PATH: ${SFML_ANDROID_PATH}")

if(NOT BOX2D_ANDROID_PATH)
	cmake_path(APPEND CMAKE_ANDROID_NDK "sources" "third_party" "box2d" OUTPUT_VARIABLE BOX2D_ANDROID_PATH)
endif()
if(NOT EXISTS ${BOX2D_ANDROID_PATH})
	message(FATAL_ERROR "BOX2D_ANDROID_PATH: \"${BOX2D_ANDROID_PATH}\" path does not exist")
endif()
if(NOT EXISTS ${BOX2D_ANDROID_PATH}/Android.mk)
	message(FATAL_ERROR "box2d Android.mk: \"${BOX2D_ANDROID_PATH}/Android.mk\" does not exist")
endif()
message(STATUS "BOX2D_ANDROID_PATH: ${BOX2D_ANDROID_PATH}")

cmake_path(APPEND BOX2D_ANDROID_PATH "lib" OUTPUT_VARIABLE BOX2D_LIB_DIR)
cmake_path(APPEND BOX2D_ANDROID_PATH "include" OUTPUT_VARIABLE BOX2D_INCLUDE_DIR)

cmake_path(APPEND SFML_ANDROID_PATH "lib" ${CMAKE_ANDROID_ARCH_ABI} OUTPUT_VARIABLE SFML_LIB_DIR)
cmake_path(APPEND SFML_ANDROID_PATH "extlibs" "lib" ${CMAKE_ANDROID_ARCH_ABI} OUTPUT_VARIABLE SFML_EXTLIB_DIR)
cmake_path(APPEND SFML_ANDROID_PATH "include" OUTPUT_VARIABLE SFML_INCLUDE_DIR)

message(VERBOSE "BOX2D_LIB_DIR: \"${BOX2D_LIB_DIR}\"")
message(VERBOSE "BOX2D_INCLUDE_DIR: \"${BOX2D_INCLUDE_DIR}\"")

message(VERBOSE "SFML_LIB_DIR: \"${SFML_LIB_DIR}\"")
message(VERBOSE "SFML_EXTLIB_DIR: \"${SFML_EXTLIB_DIR}\"")
message(VERBOSE "SFML_INCLUDE_DIR: \"${SFML_INCLUDE_DIR}\"")

function(add_library_android TARGET TYPE LOCATION_DEBUG LOCATION_RELEASE)
	if(NOT EXISTS ${LOCATION_DEBUG})
		message(FATAL_ERROR "${TARGET} ${TYPE} LOCATION_DEBUG: \"${LOCATION_DEBUG}\" path does not exist")
	endif()
	if(NOT EXISTS ${LOCATION_RELEASE})
		message(WARNING "${TARGET} ${TYPE} LOCATION_RELEASE: \"${LOCATION_RELEASE}\" does not exist")
		set(LOCATION_RELEASE ${LOCATION_DEBUG})
	endif()
	message(VERBOSE "${TARGET} ${TYPE} LOCATION_DEBUG: \"${LOCATION_DEBUG}\"")
	message(VERBOSE "${TARGET} ${TYPE} LOCATION_RELEASE: \"${LOCATION_RELEASE}\"")

	add_library(${TARGET} ${TYPE} IMPORTED)
	set_target_properties(${TARGET} PROPERTIES IMPORTED_LOCATION_DEBUG ${LOCATION_DEBUG})
	set_target_properties(${TARGET} PROPERTIES IMPORTED_IMPLIB_DEBUG ${LOCATION_DEBUG})
	set_target_properties(${TARGET} PROPERTIES IMPORTED_LOCATION_RELEASE ${LOCATION_RELEASE})
	set_target_properties(${TARGET} PROPERTIES IMPORTED_IMPLIB_RELEASE ${LOCATION_RELEASE})
endfunction()

add_library_android(box2d::box2d SHARED "${BOX2D_LIB_DIR}/libbox2d.so" "${BOX2D_LIB_DIR}/libbox2d.so")
add_library_android(SFML::Main STATIC "${SFML_LIB_DIR}/libsfml-main-d.a" "${SFML_LIB_DIR}/libsfml-main.a")
add_library_android(SFML::Activity SHARED "${SFML_LIB_DIR}/libsfml-activity-d.so" "${SFML_LIB_DIR}/libsfml-activity.so")
add_library_android(SFML::System SHARED "${SFML_LIB_DIR}/libsfml-system-d.so" "${SFML_LIB_DIR}/libsfml-system.so")
add_library_android(SFML::Window SHARED "${SFML_LIB_DIR}/libsfml-window-d.so" "${SFML_LIB_DIR}/libsfml-window.so")
add_library_android(SFML::Graphics SHARED "${SFML_LIB_DIR}/libsfml-graphics-d.so" "${SFML_LIB_DIR}/libsfml-graphics.so")
add_library_android(SFML::Audio SHARED "${SFML_LIB_DIR}/libsfml-audio-d.so" "${SFML_LIB_DIR}/libsfml-audio.so")
add_library_android(openal SHARED "${SFML_EXTLIB_DIR}/libopenal.so" "${SFML_EXTLIB_DIR}/libopenal.so")

add_library(${PROJECT_TITLE} SHARED source/debug.hpp source/main.cpp source/debug.cpp)

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

set(OUTPUT_BINARY_FULLPATH "${CMAKE_BINARY_DIR}/lib${PROJECT_TITLE}.so")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/android/app/src/main/jni/Android.mk.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/android/app/src/main/jni/Android.mk")
