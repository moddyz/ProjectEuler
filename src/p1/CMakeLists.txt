get_filename_component(EXECUTABLE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB CPPFILES *.cpp)
cpp_executable(${EXECUTABLE_NAME}
    CPPFILES
        ${CPPFILES}
)
