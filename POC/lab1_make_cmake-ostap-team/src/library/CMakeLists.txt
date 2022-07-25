cmake_minimum_required(VERSION 3.16)
project(zlib C)

set(CMAKE_C_STANDARD 99)
# Source files
set(zlib_SOURCE_FILES adler32.c crc32.c gzclose.c gzread.c infback.c inflate.c trees.c zutil.c compress.c deflate.c gzlib.c gzwrite.c inffast.c inftrees.c uncompr.c)
# For Windows :(
if(MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

# Add the Static lib and install it to bin
add_library(zlib STATIC ${zlib_SOURCE_FILES} ${zlib_HEADER_FILES})
target_include_directories(zlib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# Add the dynamic lib(Loaded it with a different nake and then renamed cuz it wouldn't let me do othervise)
# Also install the lib shared to bin
add_library(zlib-shared SHARED ${zlib_SOURCE_FILES} ${zlib_HEADER_FILES})
target_include_directories(zlib-shared PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(zlib-shared PROPERTIES OUTPUT_NAME zlib)
