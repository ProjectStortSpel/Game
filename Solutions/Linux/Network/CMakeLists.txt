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
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Renderer" "${CMAKE_CURRENT_BINARY_DIR}/Renderer")
# Link subproject shared libraries
#target_link_libraries(Network
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
