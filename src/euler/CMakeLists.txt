file(GLOB PUBLIC_HEADERS *.h)
file(GLOB CPPFILES *.cpp)

cpp_shared_library(euler
    PUBLIC_HEADERS_INSTALL_PREFIX
        euler

    PUBLIC_HEADERS
        ${PUBLIC_HEADERS}

    CPPFILES
        ${CPPFILES}
)
