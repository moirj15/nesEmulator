project(nesEmulator)
add_executable(
	nesEmulator
	main.cpp
	utils.h
	utils.cpp
	cpu.h
	cpu.cpp
    instructions.h
    instructions.cpp
    debugger.h
    debugger.cpp
)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -pedantic -g")
#set(CMAKE_EXE_LINKER_FLAGS "-lsdl2")
if (WIN32) 
target_link_libraries(
	nesEmulator
	D:/libraries/SDL2-2.0.9/lib/x64/SDL2.lib
	D:/libraries/SDL2-2.0.9/lib/x64/SDL2main.lib
)
else()
target_link_libraries(
	nesEmulator
    SDL2
)

endif()
