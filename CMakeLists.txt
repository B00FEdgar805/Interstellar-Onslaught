cmake_minimum_required(VERSION 3.20)

project(GameTestSDL3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Automatically add Homebrew's installation prefix on macOS.
# This usually resolves to /opt/homebrew on Apple Silicon or /usr/local on Intel Macs.
if(APPLE)
    find_program(BREW_EXECUTABLE brew)
    if(BREW_EXECUTABLE)
        execute_process(
            COMMAND "${BREW_EXECUTABLE}" --prefix
            OUTPUT_VARIABLE HOMEBREW_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        list(PREPEND CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
    endif()
endif()

# Find the SDL3 libraries installed on the system.
if(EMSCRIPTEN)
    set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
    set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
    set(SDLMIXER_VENDORED ON CACHE BOOL "" FORCE)

    # Disable optional MIDI/FluidSynth support for WebAssembly.
    set(SDLMIXER_MIDI OFF CACHE BOOL "" FORCE)
    set(SDLMIXER_MIDI_FLUIDSYNTH OFF CACHE BOOL "" FORCE)

    add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
    add_subdirectory(external/SDL_ttf EXCLUDE_FROM_ALL)
    add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
    add_subdirectory(external/SDL_mixer EXCLUDE_FROM_ALL)
else()
    find_package(SDL3 CONFIG REQUIRED)
    find_package(SDL3_ttf CONFIG REQUIRED)
    find_package(SDL3_image CONFIG REQUIRED)
    find_package(SDL3_mixer CONFIG REQUIRED)
endif()


# Collect all .cpp files under src/, including src/ECS and src/imgui.
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(GameTestSDL3 ${GAME_SOURCES})

if(EMSCRIPTEN)
    set_target_properties(GameTestSDL3 PROPERTIES
        SUFFIX ".html"
    )
endif()

target_include_directories(GameTestSDL3 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ECS"
)

target_link_libraries(GameTestSDL3 PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
)

if(EMSCRIPTEN)
    target_link_options(GameTestSDL3 PRIVATE
        "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/Assets@/Assets"
        "-sALLOW_MEMORY_GROWTH=1"
    )
endif()

# Keep runtime assets beside the executable in the build directory.
add_custom_command(TARGET GameTestSDL3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
        "$<TARGET_FILE_DIR:GameTestSDL3>/Assets"
    COMMENT "Copying game assets"
)

# On Windows, copy the SDL runtime DLLs beside the executable.
if(WIN32)
    foreach(SDL_TARGET
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
    )
        add_custom_command(TARGET GameTestSDL3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:${SDL_TARGET}>"
                "$<TARGET_FILE_DIR:GameTestSDL3>"
        )
    endforeach()
endif()

# Put macOS resources in the application bundle when building on macOS.
if(APPLE)
    set_target_properties(GameTestSDL3 PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "GameTestSDL3"
    )
endif()
