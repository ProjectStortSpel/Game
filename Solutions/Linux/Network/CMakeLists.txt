# Minimum required CMake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project name
project("Network")

# Create shared library
file(GLOB_RECURSE NETWORK_SRC_FILES
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Network/*.h
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Network/*.cpp)
add_library(Network SHARED ${NETWORK_SRC_FILES})

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
#if(NOT TARGET LuaEmbedder)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../LuaEmbedder" LuaEmbedder)
#endif()
#if(NOT TARGET MPL)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../MPL" MPL)
#endif()
#if(NOT TARGET Renderer)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Renderer" Renderer)
#endif()
# Link subproject shared libraries
#target_link_libraries(Network
#		      Console
#		      ECSL
#		      Input
#		      LuaEmbedder
#		      MPL
#		      Renderer)

if (WIN32)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Windows"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
  
  # Link external dynamic libraries
  target_link_libraries(Network
			${EXTERNAL_WINDOWS_DYNAMIC_LIBRARIES})
elseif (UNIX)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Linux"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
	
  # Link external shared libraries
  target_link_libraries(Network
			${EXTERNAL_UNIX_SHARED_LIBRARIES})
endif ()
