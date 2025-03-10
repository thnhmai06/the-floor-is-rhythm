# CMakeList.txt : CMake project for the-floor-is-rhythm, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (the-floor-is-rhythm)

# Require
find_package(SDL2 REQUIRED)

# Add source code files
include_directories("include")
file(GLOB SOURCES "src/*.cpp")

# Add source to this project's executable.
add_executable(main ${SOURCES})
if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET main PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
