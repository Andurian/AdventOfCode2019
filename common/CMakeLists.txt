CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT( common )

SET(TEST_PROJECT_NAME ${PROJECT_NAME}_test)

# TODO: Auto generate
SET(RESOURCE_FILES
	test_resources/file_01.txt
	test_resources/file_02.txt
	test_resources/file_03.txt
	test_resources/file_04.txt
)

ADD_LIBRARY( ${PROJECT_NAME}
	read_file.h
	read_file.cpp
	find_optional.h
	function_from_lambda.h
	infix_ostream_iterator.h
	console_image.h
	console_image.cpp
	n_comparator.h
	multicall.h
	enumerate.h
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	${EIGEN_PKG}
)

ADD_EXECUTABLE(${TEST_PROJECT_NAME}
	test.cpp
	${RESOURCE_FILES}
)

TARGET_LINK_LIBRARIES(${TEST_PROJECT_NAME}
	${PROJECT_NAME}
	CONAN_PKG::boost_test
	${EIGEN_PKG}
)

ADD_TEST(test_${TEST_PROJECT_NAME} ${TEST_PROJECT_NAME})

SET_TARGET_PROPERTIES( ${PROJECT_NAME} ${TEST_PROJECT_NAME} PROPERTIES
	FOLDER ${PROJECT_NAME}
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}
	)

SOURCE_GROUP( "Resources" FILES ${RESOURCE_FILES} )

FOREACH(INPUT ${RESOURCE_FILES})
	GET_FILENAME_COMPONENT(FILENAME ${INPUT} NAME)
	CONFIGURE_FILE(${INPUT} "${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}" COPYONLY)
ENDFOREACH()
