# CMakeList.txt : CMake project for DazaiRenderer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
find_package(OpenGL REQUIRED)
set(DEVELOPMENT_BUILD true) # set accordingly

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


add_subdirectory("vendor/glfw-3.4")
project ("DazaiRenderer")
set(CMAKE_CXX_STANDARD 20)



if(DEVELOPMENT_BUILD)
	set(RESOURCES_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources/")
else()
	set(RESOURCES_DIRECTORY "resources/")
endif()


add_definitions(-DRESOURCES="${RESOURCES_DIRECTORY}")

# Define MY_SOURCES to be a list of all the source files for my project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

#file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

add_executable (DazaiRenderer "${MY_SOURCES}" "include/glad/glad.h" "src/core/glad.c") #if lib is not with cmake

target_include_directories(DazaiRenderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

target_link_libraries(DazaiRenderer PRIVATE glfw )  # keep adding libs here

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DazaiRenderer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
