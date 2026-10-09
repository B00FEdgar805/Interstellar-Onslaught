# Install script for directory: /Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/libxmp

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/libxmp-build/libxmp.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxmp/libxmp-static-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxmp/libxmp-static-targets.cmake"
         "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/libxmp-build/CMakeFiles/Export/32d6451b784d96d0e1d28c61293ecb08/libxmp-static-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxmp/libxmp-static-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxmp/libxmp-static-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxmp" TYPE FILE FILES "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/libxmp-build/CMakeFiles/Export/32d6451b784d96d0e1d28c61293ecb08/libxmp-static-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxmp" TYPE FILE FILES "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/libxmp-build/CMakeFiles/Export/32d6451b784d96d0e1d28c61293ecb08/libxmp-static-targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxmp" TYPE FILE FILES
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/libxmp/libxmp-config.cmake"
    "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/libxmp-build/libxmp-config-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/external/SDL_mixer/external/libxmp/include/xmp.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/libxmp-build/libxmp.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/libxmp-build/examples/cmake_install.cmake")
  include("/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/libxmp-build/docs/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/edgaralamillo/Downloads/Prorgams/Personal/Interstellar-Onslaught/build-web/external/SDL_mixer/external/libxmp-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
