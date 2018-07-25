project(nesEmulator)
add_executable(
	nesEmulator
	main.cpp
	utils.h
	utils.cpp
	cpu.h
	cpu.cpp
    debugger.h
    debugger.cpp
)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -pedantic")
set(CMAKE_EXE_LINKER_FLAGS "-lSDL2")
target_link_libraries(
	nesEmulator
	SDL2
)
