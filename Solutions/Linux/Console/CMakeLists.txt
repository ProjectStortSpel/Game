# Minimum required CMake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project name
project("Console")

# Create shared library
file(GLOB_RECURSE CONSOLE_SRC_FILES
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Console/*.h
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Console/*.cpp)
add_library(Console SHARED ${CONSOLE_SRC_FILES})

# Add subprojects
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Renderer" "${CMAKE_CURRENT_BINARY_DIR}/Renderer")
# Link subproject shared libraries
#target_link_libraries(Console
#		      Renderer)

if(WIN32)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Windows"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
		      
  # Link external dynamic libraries
  target_link_libraries(Console
			${EXTERNAL_WINDOWS_DYNAMIC_LIBRARIES})
elseif(UNIX)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Linux"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")

  # Link external shared libraries
  target_link_libraries(Console
			${EXTERNAL_UNIX_SHARED_LIBRARIES})
endif()
