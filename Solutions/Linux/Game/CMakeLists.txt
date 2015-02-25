# Minimum required CMake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project name
project("Game")

# Create executable
file(GLOB_RECURSE GAME_SRC_FILES
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Game/*.h
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Game/*.cpp)
add_executable(Game ${GAME_SRC_FILES})

# Add subprojects
if(NOT TARGET Audio)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Audio" Audio)
endif()
if(NOT TARGET Console)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Console" Console)
endif()
if(NOT TARGET ECSL)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../ECSL" ECSL)
endif()
if(NOT TARGET Input)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Input" Input)
endif()
if(NOT TARGET LuaEmbedder)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../LuaEmbedder" LuaEmbedder)
endif()
if(NOT TARGET MPL)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../MPL" MPL)
endif()
if(NOT TARGET Network)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Network" Network)
endif()
if(NOT TARGET Renderer)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Renderer" Renderer)
endif()
# Link subproject shared libraries
target_link_libraries(Game
		      Audio
		      Console
		      ECSL
		      Input
		      LuaEmbedder
		      MPL
		      Network
		      Renderer)

if (WIN32)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Windows"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
		      
  # Link external dynamic libraries
  target_link_libraries(Game
			${EXTERNAL_WINDOWS_DYNAMIC_LIBRARIES}
			${EXTERNAL_WINDOWS_STATIC_SDL2MAIN_LIBRARY})
elseif (UNIX)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Linux"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
	
  # Link external shared libraries
  target_link_libraries(Game
			${EXTERNAL_UNIX_SHARED_LIBRARIES}
			${EXTERNAL_UNIX_STATIC_SDL2MAIN_LIBRARY})
endif ()
