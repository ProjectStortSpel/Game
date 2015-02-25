# Minimum required CMake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project name
project("Audio")

# Create shared library
file(GLOB_RECURSE AUDIO_SRC_FILES
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Audio/*.h
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Audio/*.cpp)
add_library(Audio SHARED ${AUDIO_SRC_FILES})

if (WIN32)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Windows"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
		      
  # Link external dynamic libraries
  target_link_libraries(Audio
			${EXTERNAL_WINDOWS_DYNAMIC_LIBRARIES}
			${EXTERNAL_WINDOWS_STATIC_LUA52_LIBRARY})
elseif (UNIX)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Linux"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
	
  # Link external shared libraries
  target_link_libraries(Audio
			${EXTERNAL_UNIX_SHARED_LIBRARIES}
			${EXTERNAL_UNIX_STATIC_LUA_LIBRARY})
endif ()
