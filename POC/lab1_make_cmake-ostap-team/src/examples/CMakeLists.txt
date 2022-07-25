cmake_minimum_required(VERSION 3.16)
project(examples C)

set(CMAKE_C_STANDARD 99)
# Sources
set(EXAMPLE_SOURCE example.c)
set(MINIGZIP_SOURCE minigzip.c)

# A special for Windows users.
if(MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

# Add the shared library
add_library(libzlib SHARED IMPORTED)
# Modify the path so it works both for windows and linux
if (WIN32)
    set_property(TARGET libzlib PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../library/build/libzlib.dll)
else()
    set_property(TARGET libzlib PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../library/build/libzlib.so)
endif()

# Add the example executable and the header path
add_executable(example ${EXAMPLE_SOURCE})
target_link_libraries(example libzlib)
target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../library)
# Add the minigzip executable and the header path
add_executable(minigzip ${MINIGZIP_SOURCE})
target_link_libraries(minigzip libzlib)
target_include_directories(minigzip PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../library)

