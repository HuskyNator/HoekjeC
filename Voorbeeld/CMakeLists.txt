cmake_minimum_required(VERSION 3.10.0)

project(VOORBEELD VERSION 0.3.1)
add_compile_definitions(VOORBEELD_VERSIE="${PROJECT_VERSION}")

file(GLOB VOORBEELD_BRON CONFIGURE_DEPENDS bron/*.c bron/*.h)

add_executable(VOORBEELD ${VOORBEELD_BRON} ${LOGO})

target_link_libraries(VOORBEELD PUBLIC -static KERN)

set_target_properties(VOORBEELD PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}
					  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
					  #OUTPUT_NAME <naam>
)
