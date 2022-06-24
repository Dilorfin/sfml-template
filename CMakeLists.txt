cmake_minimum_required(VERSION 3.10)

project("Native test library" VERSION 0.1 LANGUAGES CXX)

string(TOLOWER ${CMAKE_PROJECT_NAME} PROJECT_TITLE)
string(REGEX REPLACE "[ \t\r\n]" "-" PROJECT_TITLE ${PROJECT_TITLE})
message(VERBOSE "PROJECT_TITLE: ${PROJECT_TITLE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(BUILD_TESTS "Building tests require doctest library" OFF)
option(BUILD_BENCHMARKS "Building benhmarks require nanobench library" OFF)

option(COPY_ASSETS "Copy assets (have different behavior for android)" ON)
option(COPY_SFML_DLLS "For windows only, copy dlls from SFML_DIR/bin and error not found" ON)

#option(ANDROID_PACKAGE_NAME "A valid Java identifier for APK package" )
set(ANDROID_PACKAGE_NAME "org.dilorfin.template")
option(SFML_ANDROID_PATH "Path to prebuild SFML binaries for Android")
option(BOX2D_ANDROID_PATH "Path to prebuild box2d binaries for Android")

if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES Debug Release)
	set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)
elseif(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Release"))
	message(STATUS "CMAKE_BUILD_TYPE was \"${CMAKE_BUILD_TYPE}\", setting \"Debug\" value")
	set(CMAKE_BUILD_TYPE "Debug")
endif()

cmake_path(APPEND CMAKE_SOURCE_DIR "assets" OUTPUT_VARIABLE ASSETS_DIR)

add_subdirectory(sources)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
	add_compile_definitions(ANDROID_OS)
	add_subdirectory(android)
	return()
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	add_compile_definitions(WINDOWS_OS)
else()
	add_compile_definitions(LINUX_OS)
endif()

find_package(SFML 3 COMPONENTS System Window Graphics REQUIRED) # graphics audio network system window
find_package(box2d REQUIRED)
message(STATUS "Found box2d in ${box2d_DIR}")

set(GENERAL_DEPENDENCIES box2d::box2d SFML::System SFML::Window SFML::Graphics) # SFML::<component>

add_executable(${PROJECT_TITLE} ${SOURCE_LIST})
target_link_libraries(${PROJECT_TITLE} ${GENERAL_DEPENDENCIES}) # SFML::<component>

if(BUILD_TESTS)
	find_package(doctest REQUIRED)
	message(STATUS "Found doctest in ${doctest_DIR}")

	enable_testing()

	add_subdirectory(tests)
endif()

if(BUILD_BENCHMARKS)
	find_package(nanobench REQUIRED)
	message(STATUS "Found nanobench in ${nanobench_DIR}")

	add_subdirectory(benchmarks)
endif()

if(COPY_ASSETS)
	message(VERBOSE "ASSETS_DIR: ${ASSETS_DIR}")
	message(VERBOSE "ASSETS_DEST_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
	
	file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(COPY_SFML_DLLS AND ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	cmake_path(SET SFML_DLLS_DIR NORMALIZE "${SFML_DIR}/../../../bin")
	if(NOT EXISTS ${SFML_DLLS_DIR})
		message(STATUS "can't copy sfml dlls, ${SFML_DLLS_DIR} does not exists")
	else()
		file(GLOB_RECURSE SFML_DLLS "${SFML_DLLS_DIR}/*.dll")
		
		message(VERBOSE "SFML_DLLS_DIR: ${SFML_DLLS_DIR}")
		message(DEBUG "SFML_DLLS: ${SFML_DLLS}")

		file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	endif()
endif()

# Prettify IDEs Projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# Visual Studio Startup Project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_TITLE})
# sourse file grouping for IDEs
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})
