# CMakeList.txt : CMake project for MontyHallProblem, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()







project ("NerualNetwork")

# Add source to this project's executable.
file(GLOB MY_SOURCES "src/*")
add_executable (NerualNetwork ${MY_SOURCES} "src/layer.cpp" "src/layer.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET NerualNetwork PROPERTY CXX_STANDARD 20)
endif()


target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC NETWORKS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/networks/")
