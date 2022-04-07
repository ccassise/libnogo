list(APPEND tests
	board
	protocol
)

foreach(test IN LISTS tests)
	add_executable(test_nogo_${test} test_nogo_${test}.c ${SRC_FILES})

	set_property(TARGET test_nogo_${test} PROPERTY C_STANDARD ${C_STD})

	target_compile_options(test_nogo_${test} PRIVATE ${WFLAGS} ${SANITIZERS})
	target_include_directories(test_nogo_${test} PRIVATE ${PROJECT_SOURCE_DIR}/include)
	target_link_options(test_nogo_${test} PRIVATE ${SANITIZERS} ${SANITIZER_LIB})

	add_test(NAME test_nogo_${test} COMMAND test_nogo_${test})
endforeach()
