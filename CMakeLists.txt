CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

# Set project
SET(PROJECTNAME Mine-imator)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "-O0 -ggdb")
PROJECT(${PROJECTNAME})

# Add files
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/src")
FILE(GLOB_RECURSE SRCS "src/*.cpp")
FILE(GLOB_RECURSE HDRS "src/*.hpp")
ADD_EXECUTABLE(${PROJECTNAME} ${SRCS} ${HDRS})

# Generate object file containing resources on build
ADD_CUSTOM_COMMAND(
    OUTPUT "res.o"
    COMMAND zip -r "${CMAKE_CURRENT_BINARY_DIR}/res.zip" *
    COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}" && ld -r -b binary -o "res.o" "res.zip"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res"
)
SET_SOURCE_FILES_PROPERTIES(
    "res.o"
    PROPERTIES
    EXTERNAL_OBJECT true
    GENERATED true
)

# Add Base
INCLUDE_DIRECTORIES("dep/base")
FILE(GLOB_RECURSE BASE_SRCS "dep/base/*.cpp")
ADD_LIBRARY(base STATIC ${BASE_SRCS} "res.o")
SET(LIBS ${LIBS} base)

# Add SOIL
INCLUDE_DIRECTORIES("dep/soil")
FILE(GLOB SOIL_SRCS "dep/soil/*.c")
ADD_LIBRARY(soil STATIC ${SOIL_SRCS})
SET(LIBS ${LIBS} soil)

# Add GLFW
ADD_SUBDIRECTORY("dep/glfw")
INCLUDE_DIRECTORIES("dep/glfw/include")
SET(LIBS ${LIBS} glfw)

# Add GLEW
INCLUDE_DIRECTORIES("dep/glew/include")
FILE(GLOB GLEW_SRCS "dep/glew/src/*.c")
ADD_LIBRARY(glew STATIC ${GLEW_SRCS})
SET(LIBS ${LIBS} glew opengl32)

# Add Libzip
INCLUDE_DIRECTORIES("dep/libzip/lib")
FILE(GLOB LIBZIP_SRCS "dep/libzip/lib/*.c")
ADD_LIBRARY(libzip STATIC ${LIBZIP_SRCS})
SET(LIBS ${LIBS} libzip)

# Add Zlib
INCLUDE_DIRECTORIES("dep/zlib")
FILE(GLOB ZLIB_SRCS "dep/zlib/*.c")
ADD_LIBRARY(zlib STATIC ${ZLIB_SRCS})
SET(LIBS ${LIBS} zlib)

# Add FreeType
ADD_SUBDIRECTORY("dep/freetype")
INCLUDE_DIRECTORIES("dep/freetype/include")
SET(LIBS ${LIBS} freetype)

# Finally, link the libraries
MESSAGE("LIBS = ${LIBS}")
TARGET_LINK_LIBRARIES(${PROJECTNAME} ${LIBS})