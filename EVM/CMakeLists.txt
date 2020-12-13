cmake_minimum_required(VERSION 3.10)
project(EVM)

set(CMAKE_CXX_STANDARD 14)

add_executable(EVM main.cpp mySimpleComputer.cpp mySimpleComputer.h myTerm.cpp myTerm.h myTermColors.h myBigChars.cpp myBigChars.h myReadKey.cpp myReadKey.h keys.h timersHandler.cpp timersHandler.h CU.cpp CU.h command.h ALU.cpp ALU.h)