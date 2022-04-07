add_library(libnogo STATIC ${SRC_FILES})

target_compile_options(libnogo PRIVATE ${WFLAGS} ${SANITIZERS})
target_include_directories(libnogo PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_options(libnogo PRIVATE ${SANITIZERS} ${SANITIZER_LIB})
