# Minimum required CMake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project name
project("LuaEmbedder")

# Create shared library
file(GLOB_RECURSE LUAEMBEDDER_SRC_FILES
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/LuaEmbedder/*.h
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/LuaEmbedder/*.cpp)
add_library(LuaEmbedder SHARED ${LUAEMBEDDER_SRC_FILES})

# Add subprojects
#if(NOT TARGET Console)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Console" Console)
#endif()
#if(NOT TARGET ECSL)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../ECSL" ECSL)
#endif()
#if(NOT TARGET Input)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Input" Input)
#endif()
#if(NOT TARGET MPL)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../MPL" MPL)
#endif()
#if(NOT TARGET Network)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Network" Network)
#endif()
#if(NOT TARGET Renderer)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Renderer" Renderer)
#endif()
# Link subproject shared libraries
#target_link_libraries(LuaEmbedder
#		      Console
#		      ECSL
#		      Input
#		      MPL
#		      Network
#		      Renderer)

if (WIN32)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Windows"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
		      
  # Link external dynamic libraries
  target_link_libraries(LuaEmbedder
			${EXTERNAL_WINDOWS_DYNAMIC_LIBRARIES}
			${EXTERNAL_WINDOWS_STATIC_LUA52_LIBRARY})
elseif (UNIX)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Linux"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
	
  # Link external shared libraries
  target_link_libraries(LuaEmbedder
			${EXTERNAL_UNIX_SHARED_LIBRARIES}
			${EXTERNAL_UNIX_STATIC_LUA_LIBRARY})
endif ()
