cmake_minimum_required(VERSION 3.0.0)
project(libnogo VERSION 1.0.0 LANGUAGES C)

include(CTest)
enable_testing()

option(LIBNOGO_ENABLE_SANITIZERS "Compile with or without sanitizers" OFF)
option(LIBNOGO_ENABLE_TESTS "Determine if tests should be included or not" ON)

set(C_STD 99)
set(SANITIZERS)
set(SANITIZER_LIB)

list(APPEND WFLAGS
	-Wall
	-Wextra
	-Wshadow
	-Wdouble-promotion
	-Wconversion
	-Wpedantic
	-Wcast-align
	-Wstrict-prototypes

	-Wwrite-strings
	-Winit-self
	-Wformat=2
	-Wstrict-overflow=2
	-Wcast-qual
	-Wundef
	-Wswitch-default
	-fstack-protector-strong
	-Wparentheses
	-Wunused-macros
	-Wswitch-enum
)

if (${LIBNOGO_ENABLE_SANITIZERS})
	message(STATUS "LIBNOGO: compiling with sanitizers")
	list(APPEND SANITIZERS
		-fsanitize=address
		-fsanitize=undefined
		-fno-sanitize-recover=all
		-fsanitize=float-divide-by-zero
		-fsanitize=float-cast-overflow
		-fno-sanitize=null
		-fno-sanitize=alignment
	)

	if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
		set(SANITIZER_LIB -static-libsan)
	else()
		set(SANITIZER_LIB -static-libasan)
	endif()
endif()

list(APPEND SRC_FILES
	${PROJECT_SOURCE_DIR}/src/board.c
	${PROJECT_SOURCE_DIR}/src/protocol.c
)

add_subdirectory(src)

if (${LIBNOGO_ENABLE_TESTS})
	message(STATUS "LIBNOGO: enabling tests")
	add_subdirectory(test)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
