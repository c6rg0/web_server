set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(web-server
    main.cpp
    server.cpp
    request.cpp
    response.cpp
    config.cpp
)

option(ENABLE_TESTS "Enable tests" OFF)
if (${ENABLE_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
