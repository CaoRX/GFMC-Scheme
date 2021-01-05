cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Effective CXX)
set (CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCE_FILES
	${PROJECT_SOURCE_DIR}/src/*.cpp
	${PROJECT_SOURCE_DIR}/include/*.hpp
)
message(STATUS $ENV{SOURCE_FILES})
add_library(EffectiveLib STATIC ${SOURCE_FILES})

add_executable(run.out run/run.cpp)
target_link_libraries(run.out EffectiveLib)