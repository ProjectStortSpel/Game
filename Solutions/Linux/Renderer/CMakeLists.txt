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
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Renderer" "${CMAKE_CURRENT_BINARY_DIR}/Renderer")
# Link subproject shared libraries
#target_link_libraries(Renderer
#		      Renderer)

if (WIN32)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Windows"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
elseif (UNIX)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Linux"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
	
  # Link external shared libraries
  target_link_libraries(Renderer
			${EXTERNAL_SHARED_LIBRARIES})
endif ()
