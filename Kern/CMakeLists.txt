cmake_minimum_required(VERSION 3.10.0)
#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(KERN VERSION 0.3.1)

#configure_file(...) 'Then, configure a header file to pass the version number to the source code:'

file(GLOB_RECURSE KERN_BRON bron/*.c bron/*.h)
file(GLOB LIB lib/*.a)

add_library(KERN STATIC ${KERN_BRON} ${LIB})

target_include_directories(KERN PRIVATE bron/HC)
target_include_directories(KERN PUBLIC bron)
target_include_directories(KERN PUBLIC include)
target_link_directories(KERN PUBLIC lib)
target_link_libraries(KERN PUBLIC glew32 glfw3dll gdi32 opengl32)

add_compile_definitions(GLEW_STATIC)
