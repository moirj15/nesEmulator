project(testNesEmulator)
add_executable(
    testNesEmulator
    main.cpp
    )
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_EXE_LINKER_FLAGS "-lgtest")
target_link_libraries(
    testNesEmulator
    )
