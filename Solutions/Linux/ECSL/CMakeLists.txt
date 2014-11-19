# Minimum required CMake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project name
project("ECSL")

# Create shared library
file(GLOB_RECURSE ESCL_SRC_FILES
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/ECSL/*.h
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/ECSL/*.cpp)
add_library(ECSL SHARED ${ESCL_SRC_FILES})

# Add subprojects
if(NOT TARGET Renderer)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Renderer" Renderer)
endif()
# Link subproject shared libraries
target_link_libraries(ECSL
		      Renderer)

if (WIN32)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Windows"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
elseif (UNIX)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Linux"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
	
  # Link external shared libraries
  target_link_libraries(ECSL
			${EXTERNAL_SHARED_LIBRARIES})
endif ()
