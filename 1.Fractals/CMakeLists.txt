cmake_minimum_required(VERSION 3.8)
project(1_Fractals)

set(CMAKE_C_STANDARD 99)
set(GLUT_INCLUDE_DIR "C:/Users/peron/Downloads/glut-3.7.6-bin/glut-3.7.6-bin/include")

set(GLUT_LIBRARY "C:/Windows/System32/glut32.dll")
set(OPENGL_LIBRARIES "C:/Windows/System32/openGL32.dll")
set(GLU_LIBRARY "C:/Windows/System32/glu32.dll")

find_package(OpenGL REQUIRED)
#find_package(GLUT )

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIR} )

set(SOURCE_FILES main.c)
add_executable(1_Fractals ${SOURCE_FILES})

target_link_libraries(1_Fractals ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLU_LIBRARY} -lm )