cmake_minimum_required(VERSION 3.0)

# Define the project name of these source code
project (MedianandMean)

# We use a CMake module to fine the SystemC library. Define the module path
set (CMAKE_PREFIX_PATH $ENV{SYSTEMC_HOME})

# Find the package, which defines variable SYSTEMC_INCLUDE_DIRS
# and SYSTEMC_LIBRARIES.
find_package(SystemCLanguage CONFIG REQUIRED)

# Make Compiler option aligned to SystemC
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD} CACHE STRING
     "C++ standard to build all targets. Supported values are 98, 11, and 14.")
set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED} CACHE BOOL
     "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement.")

# Add optimization & warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")

# Set clock period = 1 ns
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D CLOCK_PERIOD=1.0")

# Define the executable name and its source code
file(GLOB SRC "*.cpp")
add_executable(MedianandMean ${SRC})

# Define the used libraries of the executable, Equal to -l flags of g++
# In here, the libraries are given in apsolute path. hence no needs for -L
# flag
target_link_libraries(MedianandMean SystemC::systemc)

# Add test mm filter command
set(OUT_FILE "house_noise_out.bmp")
add_custom_command(OUTPUT ${OUT_FILE}
	COMMAND ./MedianandMean ../house_noise.bmp ./${OUT_FILE}
	DEPENDS MedianandMean
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}	
)

set(OUT_FILE2 "cameraman_noise_out.bmp")
add_custom_command(OUTPUT ${OUT_FILE2}
	COMMAND ./MedianandMean ../cameraman_noise.bmp ./${OUT_FILE2}
	DEPENDS MedianandMean
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}	
)

add_custom_target(run 
	DEPENDS ${OUT_FILE} ${OUT_FILE2}
)

# Compare output & golden
add_custom_target(check
	COMMAND diff -s ${CMAKE_BINARY_DIR}/${OUT_FILE} golden/house.bmp
	DEPENDS run
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}	
)
