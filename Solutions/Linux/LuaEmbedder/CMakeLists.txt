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
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Renderer" "${CMAKE_CURRENT_BINARY_DIR}/Renderer")
# Link subproject shared libraries
#target_link_libraries(LuaEmbedder
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
