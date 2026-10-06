cmake_minimum_required(VERSION 3.16)
project(InterstellarOnslaught CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# Platform-specific settings
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find required packages
find_package(SDL3 REQUIRED COMPONENTS SDL3-3.0)
find_package(SDL3_image REQUIRED COMPONENTS SDL3_image-3.0)
find_package(SDL3_mixer REQUIRED COMPONENTS SDL3_mixer-3.0)
find_package(SDL3_ttf REQUIRED COMPONENTS SDL3_ttf-3.0)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Math.cpp
    src/Map.cpp
    src/TextureManager.cpp
    src/TextManager.cpp
    src/AudioManager.cpp
    src/ECS/Registry.cpp
    src/ECS/Systems.cpp
    src/ECS/CollisionSystem.cpp
    src/ECS/ProjectileSystem.cpp
    src/ECS/EnemyAI.cpp
    src/ECS/MenuSystem.cpp
    src/ECS/PlayerSystems.cpp
    src/ECS/XPSystem.cpp
    src/ECS/TileSystem.cpp
    src/ECS/Camera.cpp
    src/ECS/DamageNumberSystem.cpp
    src/ECS/Components/Transform.cpp
    src/ECS/Components/Sprite.cpp
    src/ECS/Components/Velocity.cpp
    src/ECS/Components/PlayerControl.cpp
    src/ECS/Components/BoxCollider.cpp
    src/ECS/Components/TileMap.cpp
    src/ECS/Components/Animation.cpp
    src/ECS/Components/Health.cpp
    src/ECS/Components/Enemy.cpp
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_impl_sdl3.cpp
    src/imgui/imgui_impl_sdlrenderer3.cpp
)

# Create executable
add_executable(InterstellarOnslaught ${SOURCES})

# Include directories
target_include_directories(InterstellarOnslaught PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ECS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ECS/Components
)

# Link libraries
target_link_libraries(InterstellarOnslaught PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(InterstellarOnslaught PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if(APPLE)
    # macOS specific settings if needed
    set_target_properties(InterstellarOnslaught PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

# Copy Assets to build directory on post-build
add_custom_command(TARGET InterstellarOnslaught POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    $<TARGET_FILE_DIR:InterstellarOnslaught>/Assets
    COMMENT "Copying assets to build directory"
)
