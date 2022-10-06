cmake_minimum_required(VERSION 3.24.2 FATAL_ERROR)

set(PROJECT_NAME battle-city)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp

	src/Renderer/ShaderProgram.cpp
	src/Renderer/ShaderProgram.h
	src/Renderer/Texture2D.h
	src/Renderer/Texture2D.cpp
	src/Renderer/Sprite.cpp
	src/Renderer/Sprite.h	
	src/Renderer/VertexBuffer.cpp
	src/Renderer/VertexBuffer.h
	src/Renderer/VertexArray.cpp
	src/Renderer/VertexArray.h
	src/Renderer/VertexBufferLayout.cpp
	src/Renderer/VertexBufferLayout.h
	src/Renderer/IndexBuffer.cpp
	src/Renderer/IndexBuffer.h
	src/Renderer/Renderer.h
	src/Renderer/Renderer.cpp	
	src/Renderer/SpriteAnimator.h
	src/Renderer/SpriteAnimator.cpp

	src/Resources/ResourceManager.cpp
	src/Resources/ResourceManager.h
	src/Resources/stb_image.h

	src/Game/Game.h
	src/Game/Game.cpp	
	src/Game/Level.h
	src/Game/Level.cpp	

	src/Game/GameObjects/IGameObject.h
	src/Game/GameObjects/IGameObject.cpp
	src/Game/GameObjects/Tank.h
	src/Game/GameObjects/Tank.cpp
	src/Game/GameObjects/BrickWall.cpp
	src/Game/GameObjects/BrickWall.h
	src/Game/GameObjects/BetonWall.cpp
	src/Game/GameObjects/BetonWall.h
	src/Game/GameObjects/Trees.cpp
	src/Game/GameObjects/Trees.h
	src/Game/GameObjects/Ice.cpp
	src/Game/GameObjects/Ice.h
	src/Game/GameObjects/Water.cpp
	src/Game/GameObjects/Water.h
	src/Game/GameObjects/Eagle.cpp
	src/Game/GameObjects/Eagle.h

	res/shaders/vertex.vert
	res/shaders/fragment.frag
	res/shaders/fSprite.frag
	res/shaders/vSprite.vert
	res/resources.json
)

set_target_properties(${PROJECT_NAME}  PROPERTIES
  RESOURCE "res/shaders/vertex.vert
	res/shaders/fragment.frag
	res/shaders/fSprite.frag
	res/shaders/vSprite.vert
	res/resources.json")

	
file(GLOB_RECURSE SHADERS LIST_DIRECTORIES false
	"${CMAKE_SOURCE_DIR}/res/shaders/*.frag" 
	"${CMAKE_SOURCE_DIR}/res/shaders/*.vert"
	"${CMAKE_SOURCE_DIR}/res/textures/*.png"
	"${CMAKE_SOURCE_DIR}/res/*.json"
)

source_group("Resource Files" FILES ${SHADERS})


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)


include_directories(external/rapidjson/include)
include_directories(external/glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
							COMMAND ${CMAKE_COMMAND} -E copy_directory
							${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)