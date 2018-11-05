cmake_minimum_required(VERSION 2.8)

### Specification of build type (Release or Debug) - uncomment the wanted type.
SET (CMAKE_BUILD_TYPE "Release")
#SET (CMAKE_BUILD_TYPE "Debug")

### Specification of the build execution depending on the host operating system
IF (${APPLE})	# if the kernel is Darwin's
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 -I/usr/local/include -L/usr/local/lib")
ELSE ()			# if the kernel is not Darwin's
	SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17")
ENDIF ()

### Specification of build flags, according to the chosen build type
IF(${CMAKE_CXX_FLAGS_DEBUG})
    #ENVIRONMENT (LD_PRELOAD=/usr/lib64/libasan.so)
    #SET (CMAKE_CXX_FLAGS_DEBUG "-g3")
    SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address ")
ELSE()
    SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
ENDIF ()

### Specification of BASE_DIR variable
SET (BASE_DIR ${CMAKE_SOURCE_DIR} )

### Setting output directory (of built binaries) in the same level as BASE_DIR, within a bin/tools directory
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../bin)

### Setting multiple directories with project's inclusions, such as include and lib directories
INCLUDE_DIRECTORIES(${BASE_DIR}/../lib/ ${BASE_DIR}/include/ ${BASE_DIR}/exception/ ${BASE_DIR})

### Add a new and own-made library (bstream)
ADD_LIBRARY(bstream  ${BASE_DIR}/../lib/bstream.cpp)

### Add a new and own-made library (extmath)
ADD_LIBRARY(extmath  ${BASE_DIR}/../lib/extmath.cpp)

### Add a new and own-made library (wavdec)
ADD_LIBRARY(golomb  ${BASE_DIR}/Golomb.cpp)

### Targeting libraries to further link with losslesscodec executable
target_link_libraries (golomb bstream extmath)

### Add a new and own-made library (wavdec)
ADD_LIBRARY(quantizer  ${BASE_DIR}/Quantizer.cpp)

### Add a new and own-made library (wavdec)
ADD_LIBRARY(audiohandler  ${BASE_DIR}/AudioHandler.cpp)

### Add a new and own-made library (wavdec)
ADD_LIBRARY(naivepredictor  ${BASE_DIR}/NaivePredictor.cpp)

### Add a new and own-made library (wavdec)
ADD_LIBRARY(advancedpredictor  ${BASE_DIR}/AdvancedPredictor.cpp)

### Targeting libraries to further link with losslesscodec executable
target_link_libraries (advancedpredictor quantizer)

### Marking main.cpp final binary as losslesscodec
add_executable (losslesscodec main.cpp)

### Targeting libraries to further link with losslesscodec executable
target_link_libraries (losslesscodec sndfile bstream extmath golomb audiohandler naivepredictor quantizer advancedpredictor boost_program_options boost_filesystem boost_iostreams boost_system)
