cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(KERN VERSION 0.3.1)
add_compile_definitions(GLEW_STATIC)
add_compile_definitions(KERN_VERSIE="${PROJECT_VERSION}")

file(GLOB_RECURSE KERN_BRON CONFIGURE_DEPENDS bron/*.c bron/*.h)
file(GLOB LIB CONFIGURE_DEPENDS lib/*.a)

add_library(KERN STATIC ${KERN_BRON} ${LIB})

target_include_directories(KERN PRIVATE bron/HC)
target_include_directories(KERN PUBLIC bron)
target_include_directories(KERN PUBLIC include)

find_library(GLEW glew32 HINTS ${PROJECT_SOURCE_DIR}/lib)
find_library(GLFW glfw3 HINTS ${PROJECT_SOURCE_DIR}/lib)
find_library(GLFW_DLL glfw3dll HINTS ${PROJECT_SOURCE_DIR}/lib)
find_library(GDI gdi32)
find_library(OPENGL opengl32)

target_link_libraries(KERN PUBLIC -static ${GLEW} ${GLFW} ${GLFW_DLL} ${GDI} ${OPENGL})