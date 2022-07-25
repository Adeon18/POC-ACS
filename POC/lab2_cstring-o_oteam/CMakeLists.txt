cmake_minimum_required(VERSION 3.15)

#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
set(PROJECT_NAME text_to_hex)
set(LIB_NAME my_c_string)
set(DYNAMIC_ARRAY_LIB_NAME dyn_array)

project(${PROJECT_NAME} C CXX)

#! Options
set(WARNINGS_AS_ERRORS ON)    # "Treat compiler warnings as errors."
set(ENABLE_PVS_STUDIO OFF)    # "Check using command-line PVS-Studio."
#! ENABLE_SANITIZERS is the option for the test builds!
#       Definitely enable it while developing.
#       Disable it for the production builds and before submitting for grading!
set(ENABLE_SANITIZERS OFF)    # "Use sanitizers to detect errors."

#! Build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if(MSYS)
    message(FATAL_ERROR "Your should not use MSYS shell! Use MinGW64!") # Or MinGW32 if you know what you are doing.
endif()
#####################################

#! Make your c_strings library
add_library(
  ${LIB_NAME} SHARED 
  ${CMAKE_SOURCE_DIR}/c_str_lib/c_string.c
  ${CMAKE_SOURCE_DIR}/c_str_lib/c_string.h)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/c_str_lib)

add_library(${DYNAMIC_ARRAY_LIB_NAME}
        SHARED
        ${CMAKE_SOURCE_DIR}/array_lib/dynamic_array.c
        ${CMAKE_SOURCE_DIR}/array_lib/dynamic_array.h)

target_include_directories(${DYNAMIC_ARRAY_LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/array_lib)

#####################################

#! Project source compilation
add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(${PROJECT_NAME} ${LIB_NAME})
target_link_libraries(${PROGRAM_NAME} ${LIB_NAME} ${DYNAMIC_ARRAY_LIB_NAME})

#####################################

#! Specify output directories
#! For different tests etc it should be `bin`. For shared library it should be `lib`
set_target_properties(${LIB_NAME} ${DYNAMIC_ARRAY_LIB_NAME} ${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(${LIB_NAME} ${DYNAMIC_ARRAY_LIB_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)


#####################################
# Define ALL_TARGETS variable to use in PVS and Sanitizers
set (ALL_TARGETS ${PROJECT_NAME} ${LIB_NAME} ${DYNAMIC_ARRAY_LIB_NAME})

# Include fixed CMake configuration
include(cmake/config.cmake)
 
