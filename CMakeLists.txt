project(nesEmulator)
add_executable(
	nesEmulator
	main.cpp
	utils.h
	utils.cpp
	cpu.h
	cpu.cpp
)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_EXE_LINKER_FLAGS "-lSDL2")
target_link_libraries(
	nesEmulator
	SDL2
)
