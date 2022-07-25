cmake_minimum_required(VERSION 3.15)

#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
#  It is used as your project's main executable name. 
set(PROJECT_NAME word_indexer)
project(${PROJECT_NAME} C CXX) # project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 17)

##########################################################
# User configurable options of the word_indexer
##########################################################
# Note: symbols like WARNINGS_AS_ERRORS in configuration are intentionally variables
# and not CMake options --using options creates too much problem for students.

#! It is a good practice to set "WARNINGS_AS_ERRORS" ON,
#  but sometimes it creates too much trouble, so default is OFF.
set(WARNINGS_AS_ERRORS OFF)

#! Always use PVS Studio while developing. 
set(ENABLE_PVS_STUDIO OFF)

#! Select appropriate sanitizers.
#  Definitely enable sanitizers while developing.
#  Disable it for the production builds and before submitting for grading.
#  Only one of Memory (MSAN), Address (ASAN), or Thread (TSan)
#  sanitizers is applicable at the time -- the first defined.

#! UndefinedBehaviorSanitizer (UBSan)
#  Info: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
set(ENABLE_UBSan ON)
#! AddressSanitizer -- detects use after free or after scope exit,
#  memory overflows and leaks.  
#  Info: https://github.com/google/sanitizers/wiki/AddressSanitizer
set(ENABLE_ASAN OFF)
#! ThreadSanitizer -- detects data races.
set(ENABLE_TSan OFF)
#! MemorySanitizer detects uninitialized memory reads 
#  Info: https://github.com/google/sanitizers/wiki/MemorySanitizer
set(ENABLE_MSAN OFF)


#! Be default -- build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

# Warnings as errors should be imported here -- do not move this line
include(cmake/CompilerWarnings.cmake)

##########################################################
# Project files, packages, libraries and so on
##########################################################

#! Project main executable source compilation
add_executable(${PROJECT_NAME} src/main.cpp
				mt_deque/mt_deque.hpp mt_unordered_map/mt_unordered_map_t.hpp
				options_parser/options_parser.cpp options_parser/options_parser.hpp
				word_count/word_count.cpp word_count/word_count.hpp
				toml_parser/toml_parser.cpp toml_parser/toml_parser.hpp
				include/exit_codes.hpp include/limits.hpp
				time_measurer/time_measurer.hpp)

if (DEQUE_DEBUG)
	TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PRIVATE DEQUE_DEBUG=1)
endif()

if (UNORDERED_MAP_DEBUG)
	TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PRIVATE UNORDERED_MAP_DEBUG=1)
endif()

#! Put path to your project headers
target_include_directories(${PROJECT_NAME} PRIVATE options_parser word_count include)

#! Add external packages
# options_parser requires boost::program_options. boost::algorithm needs Boost::headers
find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system Boost::headers)

#! Allow threads on linux. Source:
#! https://stackoverflow.com/questions/34143265/undefined-reference-to-symbol-pthread-createglibc-2-2-5
if (UNIX AND NOT APPLE)
	SET(CMAKE_THREAD_LIBS_INIT "-lpthread")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	SET(CMAKE_HAVE_THREADS_LIBRARY 1)
	SET(CMAKE_USE_WIN32_THREADS_INIT 0)
	SET(CMAKE_USE_PTHREADS_INIT 1)
	SET(THREADS_PREFER_PTHREAD_FLAG ON)
endif()
						
##########################################################
# Fixed CMakeLists.txt part 
##########################################################

INSTALL(PROGRAMS
		$<TARGET_FILE:${PROJECT_NAME}>	# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
		DESTINATION bin)

# Define ALL_TARGETS variable to use in PVS and Sanitizers
set(ALL_TARGETS ${PROJECT_NAME})

# Include CMake setup
include(cmake/main-config.cmake)
