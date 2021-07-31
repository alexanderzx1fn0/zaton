cmake_minimum_required(VERSION 2.8.3...3.19)
project(zaton)

set(default_USE_X11 ON)
find_package(X11)
find_package(OpenGL REQUIRED)

if(X11_FOUND)
    link_directories(${X11_LIBRARY_DIR})
    include_directories(${X11_INCLUDE_DIR})
endif()

if(WIN32)
    list(APPEND PLATFORM_SOURCES src/core/WinMain.cpp)
else()
    list(APPEND PLATFORM_SOURCES src/core/LinuxMain.cpp)
endif()


set(SOURCES_FILES_MAIN
${PLATFORM_SOURCES}

"src/core/Game.h"
"src/core/Game.cpp"
"src/core/Platform.h"
"src/core/Timer.h"
"src/core/Timer.cpp"

"src/graphics/opengl/OpenGLExtensions.h"
"src/graphics/opengl/OpenGLExtensions.cpp"
"src/graphics/opengl/OpenGLRenderer.h"
"src/graphics/opengl/OpenGLRenderer.cpp"
"src/graphics/Renderer.h"
"src/graphics/Renderer.cpp"
"src/graphics/Shader.h"
"src/graphics/Shader.cpp"
"src/graphics/data/Mesh.h"
"src/graphics/data/Mesh.cpp"
"src/graphics/Texture.h"
"src/graphics/Texture.cpp"
"src/graphics/Sprite.h"
"src/graphics/Sprite.cpp"
"src/graphics/Model.h"
"src/graphics/DrawDebug.h"
"src/graphics/DrawDebug.cpp"


"src/input/Input.h"
"src/input/Input.cpp"
"src/input/Keyboard.h"
"src/input/Mouse.h"

"src/game/Spider.h"
"src/game/Spider.cpp"
"src/game/Camera.h"
"src/game/Camera.cpp"
"src/game/BasePlayer.h"
"src/game/Player.h"
"src/game/Player.cpp"
"src/game/Entity.h"
"src/game/Entity.cpp"
"src/game/Weapon.h"
"src/game/Weapon.cpp"

"src/game/particle/particle.h"
"src/game/particle/fountainEmitter.h"
"src/game/particle/fountainEmitter.cpp"
"src/game/particle/fountain.h"
"src/game/particle/fountain.cpp"

"src/gui/UI.h"
"src/gui/UI.cpp"


"src/scene/LoadLevel.h"
"src/scene/LoadLevel.cpp"
"src/scene/Cube.h"
"src/scene/Cube.cpp"

"src/util/utils.h"
"src/util/utils.cpp"
"src/util/Stream.h"
"src/util/Stream.cpp"

"src/physics/Collision.h"
"src/physics/Collision.cpp"

"src/math/random.h"
"src/math/constant.h"
)

include_directories("libs/")
include_directories("libs/glm/")
include_directories("src/")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES_FILES_MAIN})
if (MSVC)
    target_link_libraries(zaton ${OPENGL_LIBRARIES} shell32 user32 gdi32 -NODEFAULTLIB:LIBCMT)
else()
    target_link_libraries(zaton ${OPENGL_LIBRARIES} -lm)
    set (CMAKE_CXX_FLAGS "-g -fpermissive")
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif()
    

