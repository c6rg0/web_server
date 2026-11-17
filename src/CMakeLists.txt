set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(web-server
    main.cpp
    server.cpp
    request.cpp
    response.cpp
    config.cpp
)
