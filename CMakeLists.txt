cmake_minimum_required (VERSION 3.6.1)
project(pandemic-geographical_model)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_COMPILER "g++")
add_compile_options(-g)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# With some IDEs, all of the associated projects are kept in a single folder. In that case, the location of cadmium will
# probably not be in that single folder, but in the parent folder (so parent folder / IDE Project Folder / this project).
# However, if cloning from scratch, this project will be in its own folder (so parent folder / this project).
# This if statement is used to account for both cases.

get_filename_component(cadmium "../cadmium/json" REALPATH)

if(EXISTS "${cadmium}")
    message("Cadmium not found")
    include_directories(../cadmium/include)
    include_directories(../cadmium/json/include)
else()
    message("Cadmium found")
    include_directories(../../cadmium/include)
    include_directories(../../cadmium/json/include)
endif()

set(Boost_USE_MULTITHREADED TRUE)
find_package(Boost COMPONENTS unit_test_framework system thread REQUIRED)

file(MAKE_DIRECTORY logs)

add_executable(pandemic-geographical_model src/main.cpp)

target_link_libraries(pandemic-geographical_model PUBLIC ${Boost_LIBRARIES})
