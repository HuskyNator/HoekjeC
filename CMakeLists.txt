cmake_minimum_required(VERSION 3.28.1)

project(HoekjeC
	VERSION 0.3.2
	LANGUAGES C)

file(GLOB_RECURSE HC_SOURCE_FILES CONFIGURE_DEPENDS src/*.c)

add_library(HoekjeC STATIC ${HC_SOURCE_FILES})

target_compile_definitions(HoekjeC PRIVATE GLEW_STATIC)
target_compile_definitions(HoekjeC PUBLIC KERN_VERSIE="${PROJECT_VERSION}")

target_include_directories(HoekjeC PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(HoekjeC PRIVATE ${PROJECT_SOURCE_DIR}/src/HC)
target_include_directories(HoekjeC PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_directories(HoekjeC PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(HoekjeC PUBLIC glew32s glfw3 glfw3dll gdi32 opengl32)
# file(GLOB LIB CONFIGURE_DEPENDS lib/*.a)
# find_library(GLEW glew32 HINTS ${PROJECT_SOURCE_DIR}/lib)
# find_library(GLFW glfw3 HINTS ${PROJECT_SOURCE_DIR}/lib)
# find_library(GLFW_DLL glfw3dll HINTS ${PROJECT_SOURCE_DIR}/lib)
# find_library(GDI gdi32)
# find_library(OPENGL opengl32)

set(LOGO ${PROJECT_SOURCE_DIR}/Logo/Logo.rc)

option(BUILD_EXAMPLE "Whether to build the example project" OFF)

if(BUILD_EXAMPLE)
	add_subdirectory(example)
endif()
