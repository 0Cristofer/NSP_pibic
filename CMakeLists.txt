cmake_minimum_required(VERSION 3.12)
project(NSP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE -O3)

add_executable(NSP src/nsp.cpp src/apc/apc.cpp src/apc/include/apc.hpp src/util/util.cpp src/util/include/util.hpp src/classes/NSP.cpp src/classes/include/NSP.hpp src/classes/Case.cpp src/classes/include/Case.hpp src/classes/Nurse.cpp src/classes/include/Nurse.hpp src/classes/Day.cpp src/classes/include/Day.hpp src/classes/Shift.cpp src/classes/include/Shift.hpp src/classes/Solution.cpp src/classes/include/Solution.hpp src/apc/apcsolution.cpp src/apc/include/apcsolution.hpp)