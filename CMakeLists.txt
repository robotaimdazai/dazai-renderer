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

include_directories("vendor/imgui")

add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glm")

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
add_executable (DazaiRenderer "${MY_SOURCES}" 
"vendor/glad/glad.h" 
"vendor/glad/glad.c"

"vendor/imgui/imgui.cpp"
"vendor/imgui/imgui.h"
"vendor/imgui/imconfig.h"
"vendor/imgui/imgui_demo.cpp"
"vendor/imgui/imgui_draw.cpp"
"vendor/imgui/imgui_internal.h"
"vendor/imgui/imstb_rectpack.h"
"vendor/imgui/imstb_textedit.h"
"vendor/imgui/imstb_truetype.h"
"vendor/imgui/imgui_widgets.cpp"
"vendor/imgui/imgui_tables.cpp"
"vendor/imgui/backends/imgui_impl_glfw.h"
"vendor/imgui/backends/imgui_impl_glfw.cpp"
"vendor/imgui/backends/imgui_impl_opengl3.h"
"vendor/imgui/backends/imgui_impl_opengl3.cpp"
"vendor/imgui/backends/imgui_impl_opengl3_loader.h"
) #if lib is not with cmake, if its header only then dont

target_link_libraries(DazaiRenderer PRIVATE glfw glm)  # keep adding libs here

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DazaiRenderer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
