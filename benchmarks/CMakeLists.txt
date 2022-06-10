function(ct_add_bechmark target SOURCES)
	# TODO: try once more to force bechmarks target only Release

	# create the target
	add_executable(${target} ${SOURCES} ${SOURCE_LIST_WO_MAIN})

	# sourse file grouping for IDEs
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
	source_group("${PROJECT_TITLE} sources" FILES ${SOURCE_LIST_WO_MAIN})
	
	# set the target's folder (for IDEs that support it, e.g. Visual Studio)
	set_target_properties(${target} PROPERTIES FOLDER "Bechmarks")

	# link the target to its SFML dependencies
	target_link_libraries(${target} PRIVATE nanobench::nanobench ${GENERAL_DEPENDENCIES})

	# Add the test
	#add_test(NAME ${target} COMMAND ${target} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endfunction()

ct_add_bechmark(bench1 bench1.cpp)
