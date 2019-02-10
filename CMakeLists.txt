#set(CMAKE_SYSTEM_NAME WIN32)
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
    tests.h
    tests.cpp
)
#for clang on windows
# cmake .. -G"NMake Makefiles JOM" -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -DCMAKE_LINKER="C:/Program Files/LLVM/bin/lld-link.exe" -DCMAKE_C_COMPILER_ID="Clang" -DCMAKE_CXX_COMPILER_ID="CLANG" -DCMAKE_SYSTEM_NAME="Generic"
#set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -pedantic -g")
set(CMAKE_CXX_FLAGS "/std:c++17 /MP /GS /TP /W3 -D_CRT_SECURE_NO_WARNINGS")
#set(CMAKE_EXE_LINKER_FLAGS "-lsdl2")
#if (WIN32) 
target_link_libraries(
	nesEmulator
	D:/libraries/SDL2-2.0.9/lib/x64/SDL2.lib
	D:/libraries/SDL2-2.0.9/lib/x64/SDL2main.lib
)
#else()
message("OUCH")
#target_link_libraries(
#	nesEmulator
    #SDL2
#)

#endif()
