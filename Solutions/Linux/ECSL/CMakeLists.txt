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
#if(NOT TARGET Console)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Console" Console)
#endif()
#if(NOT TARGET Input)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Input" Input)
#endif()
#if(NOT TARGET LuaEmbedder)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../LuaEmbedder" LuaEmbedder)
#endif()
if(NOT TARGET MPL)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../MPL" MPL)
endif()
#if(NOT TARGET Network)
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Network" Network)
#endif()
if(NOT TARGET Logger)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Logger" Logger)
endif()
# Link subproject shared libraries
target_link_libraries(ECSL
		      MPL
		      Logger)

if(WIN32)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Windows"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")
		      
  # Link external dynamic libraries
  target_link_libraries(ECSL
			${EXTERNAL_WINDOWS_DYNAMIC_LIBRARIES})
elseif(UNIX)
  # Include directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../Externals/include/Linux"
		      "${CMAKE_CURRENT_SOURCE_DIR}/../../../Source")

  # Link external shared libraries
  target_link_libraries(ECSL
			${EXTERNAL_UNIX_SHARED_LIBRARIES})
endif()
