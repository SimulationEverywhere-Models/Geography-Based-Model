cmake_minimum_required (VERSION 3.6.1)
project(pandemic-hoya-2002)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_COMPILER "g++")
add_compile_options(-g)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(/home/binybrion/Work_Summer2020/cadmium-json/include)
include_directories(/home/binybrion/Work_Summer2020/cadmium-json/json/include)

set(Boost_USE_MULTITHREADED TRUE)
find_package(Boost COMPONENTS unit_test_framework system thread REQUIRED)

file(MAKE_DIRECTORY logs)

add_executable(hoya src/main.cpp model/cells/Zhong_Cell.h model/Zhong_Coupled.h model/cells/Vicinity.h model/cells/SIR.h)

target_link_libraries(hoya PUBLIC ${Boost_LIBRARIES})
