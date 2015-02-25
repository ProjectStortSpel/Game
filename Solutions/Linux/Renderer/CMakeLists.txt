# Minimum required CMake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project name
project("Renderer")

# Create shared library
file(GLOB_RECURSE RENDERER_SRC_FILES
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Renderer/*.h
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Renderer/*.cpp)
add_library(Renderer SHARED ${RENDERER_SRC_FILES})

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
#if(NOT TARGET Network)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Network" Network)
#endif()
# Link subproject shared libraries
#target_link_libraries(Renderer
#		      Console
#		      ECSL
#		      Input
#		      LuaEmbedder
#		      MPL
#		      Network)

if (WIN32)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Windows"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
		      
  # Link external dynamic libraries
  target_link_libraries(Renderer
			${EXTERNAL_WINDOWS_DYNAMIC_LIBRARIES})
elseif (UNIX)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Linux"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
	
  # Link external shared libraries
  target_link_libraries(Renderer
			${EXTERNAL_UNIX_SHARED_LIBRARIES})
endif ()
