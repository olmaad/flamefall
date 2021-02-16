cmake_minimum_required (VERSION 3.2)

project(FlameFall)

# Dependencies
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Shared
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/$<CONFIG>")

# Application
add_executable(FlameFall
	src/main.cpp
	src/context_wrappers.h
	src/context_wrappers.cpp
    src/shader.h
	src/shader.cpp
	src/uniform.h
	src/uniform.cpp
	src/scene.h
	src/scene.cpp
	src/buffer.h
	src/buffer.cpp
)	
set_property(TARGET FlameFall PROPERTY FOLDER "FlameFall")
set_property(TARGET FlameFall PROPERTY CXX_STANDARD 17)
set_property(TARGET FlameFall PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(FlameFall PRIVATE GLEW::glew glm glfw assimp::assimp)
target_link_libraries(FlameFall PRIVATE GLEW::glew glm glfw assimp::assimp)

# Resources
file(GLOB_RECURSE FLAMEFALL_RES_LIST base/*)
set(FLAMEFALL_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/base")
set(FLAMEFALL_RES_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res")

add_custom_target(BuildResources
	ALL
	COMMAND ${CMAKE_COMMAND} -E make_directory "${FLAMEFALL_RES_DIR}"
	COMMAND blender "${FLAMEFALL_BASE_DIR}/Models/Cliff.blend" --background --python-expr "import bpy, sys; bpy.ops.export_scene.obj(filepath='${FLAMEFALL_RES_DIR}/Cliff.obj', check_existing=False, use_selection=True)"
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${FLAMEFALL_BASE_DIR}/Shaders" "${FLAMEFALL_RES_DIR}/Shaders"
	SOURCES ${FLAMEFALL_RES_LIST}
)
set_property(TARGET BuildResources PROPERTY FOLDER "FlameFall")
