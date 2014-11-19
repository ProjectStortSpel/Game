# Minimum required CMake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Options
set(USE_X86_64 ON CACHE BOOL "USE_X86_64")

# Project name
project("Large Game Project")

if(WIN32)
  if(CMAKE_CL_64)
    set(WINDOWS_SYSTEM "x64")
  else()
    set(WINDOWS_SYSTEM "x86")
  endif()
  
  # Define external libraries
  set(EXTERNAL_WINDOWS_DYNAMIC_LIBRARIES
      opengl32
      "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/lib/Windows/${WINDOWS_SYSTEM}/glew32.lib"
      version
      imm32
      winmm
      "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/lib/Windows/${WINDOWS_SYSTEM}/SDL2.lib"
      debug "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/lib/Windows/${WINDOWS_SYSTEM}/RakNet_x86d.lib"
      optimized "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/lib/Windows/${WINDOWS_SYSTEM}/RakNet_x86.lib")
  set(EXTERNAL_WINDOWS_STATIC_SDL2MAIN_LIBRARY
      "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/lib/Windows/${WINDOWS_SYSTEM}/SDL2main.lib")
  set(EXTERNAL_WINDOWS_STATIC_LUA52_LIBRARY
      "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/lib/Windows/${WINDOWS_SYSTEM}/lua52.lib")

  # Set output directory
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/Debug/${WINDOWS_SYSTEM}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/Debug/${WINDOWS_SYSTEM}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/Release/${WINDOWS_SYSTEM}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/Release/${WINDOWS_SYSTEM}")
  
  # Copy binaries to output directory
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/binaries/Windows/${WINDOWS_SYSTEM}"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/binaries/Windows/${WINDOWS_SYSTEM}"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
elseif(UNIX)
  if(USE_X86_64)
    set(UNIX_SYSTEM "x86_64")
  else()
    set(UNIX_SYSTEM "i386")
  endif()

  # Define external libraries
  file(GLOB EXTERNAL_UNIX_SHARED_LIBRARIES
       "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/lib/Linux/${UNIX_SYSTEM}/*.so"
       "dl")
  set(EXTERNAL_UNIX_STATIC_SDL2MAIN_LIBRARY
      "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/lib/Linux/${UNIX_SYSTEM}/libSDL2main.a")
  set(EXTERNAL_UNIX_STATIC_LUA_LIBRARY
      "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/lib/Linux/${UNIX_SYSTEM}/liblua.a")

  # Set output directory
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/Debug/${UNIX_SYSTEM}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/Debug/${UNIX_SYSTEM}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/Release/${UNIX_SYSTEM}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../Bin/Release/${UNIX_SYSTEM}")

  # Set compilation and linker flags
  if(${USE_X86_64})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -std=c++0x")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m64")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -m64")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++0x")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -m32")
  endif()

  # Set relative path to libraries for executable
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,-rpath,'$ORIGIN' -Wl,-rpath,'$ORIGIN/bin'")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,-rpath,'$ORIGIN' -Wl,-rpath,'$ORIGIN/bin'")
  
  # Copy binaries to output directory
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/bin")
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/bin")
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/binaries/Linux/${UNIX_SYSTEM}"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/bin")
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/../../Externals/binaries/Linux/${UNIX_SYSTEM}"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/bin")
endif()

# Add subprojects
#if(NOT TARGET Console)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Console" Console)
#endif()
if(NOT TARGET ECSL)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ECSL" ECSL)
endif()
if(NOT TARGET Input)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Input" Input)
endif()
if(NOT TARGET LuaEmbedder)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/LuaEmbedder" LuaEmbedder)
endif()
if(NOT TARGET MPL)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/MPL" MPL)
endif()
if(NOT TARGET Network)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Network" Network)
endif()
if(NOT TARGET Renderer)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Renderer" Renderer)
endif()

if(NOT TARGET Game)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Game" Game)
endif()
