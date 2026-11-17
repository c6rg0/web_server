project(tests)
add_executable(tests
    main.cpp 
    ../server.cpp
    ../request.cpp
    ../response.cpp
    ../config.cpp
)
target_link_libraries(tests PRIVATE gtest)
add_test(tests tests)
