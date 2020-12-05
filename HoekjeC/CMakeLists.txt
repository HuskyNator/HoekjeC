cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(HoekjeC VERSION 0.3.2)
add_compile_definitions(GLEW_STATIC)
add_compile_definitions(HoekjeC_VERSIE="${PROJECT_VERSION}")

file(GLOB_RECURSE HoekjeC_BRON CONFIGURE_DEPENDS bron/*.c bron/*.h)
file(GLOB HoekjeC_LIB CONFIGURE_DEPENDS lib/*.a)

add_library(HoekjeC STATIC ${HoekjeC_BRON} ${HoekjeC_LIB})
set_target_properties(HoekjeC PROPERTIES VERSION ${PROJECT_VERSION})

target_include_directories(HoekjeC PRIVATE bron/HC)
target_include_directories(HoekjeC PUBLIC bron)
target_include_directories(HoekjeC PUBLIC include)

find_library(GLEW glew32 HINTS ${PROJECT_SOURCE_DIR}/lib)
find_library(GLFW glfw3 HINTS ${PROJECT_SOURCE_DIR}/lib)
find_library(GLFW_DLL glfw3dll HINTS ${PROJECT_SOURCE_DIR}/lib)
find_library(GDI gdi32)
find_library(OPENGL opengl32)

target_link_libraries(HoekjeC PUBLIC -static ${GLEW} ${GLFW} ${GLFW_DLL} ${GDI} ${OPENGL})