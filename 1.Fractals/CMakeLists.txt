cmake_minimum_required(VERSION 3.8)
project(1_Fractals)

set(CMAKE_C_STANDARD 99)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

set(SOURCE_FILES main.c)
add_executable(1_Fractals ${SOURCE_FILES})

target_link_libraries(1_Fractals ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} -lm )