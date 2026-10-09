# Install script for directory: /Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/edgaralamillo/emsdk/upstream/emscripten/cache/sysroot")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/FLAC" TYPE FILE FILES
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC/all.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC/assert.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC/callback.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC/export.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC/format.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC/metadata.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC/ordinals.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC/stream_decoder.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC/stream_encoder.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/FLAC++" TYPE FILE FILES
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC++/all.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC++/decoder.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC++/encoder.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC++/export.h"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/flac/include/FLAC++/metadata.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/flac-build/src/cmake_install.cmake")
  include("/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/flac-build/microbench/cmake_install.cmake")
  include("/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/flac-build/doc/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/flac-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
