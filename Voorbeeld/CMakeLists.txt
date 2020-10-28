cmake_minimum_required(VERSION 3.10.0)

project(VOORBEELD VERSION 0.3.2)
add_compile_definitions(VOORBEELD_VERSIE="${PROJECT_VERSION}")

file(GLOB VOORBEELD_BRON CONFIGURE_DEPENDS bron/*.c bron/*.h)

add_executable(VOORBEELD ${VOORBEELD_BRON} ${LOGO})

target_link_libraries(VOORBEELD PUBLIC -static KERN)

set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/uitgave/Voorbeeld")
set_target_properties(VOORBEELD PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY})
get_target_property(KERN_SOURCE_DIR KERN SOURCE_DIR)

add_custom_command(TARGET VOORBEELD POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Ververs ${RUNTIME_OUTPUT_DIRECTORY}/Voorbeeld/Ververs
					COMMENT "Ververs Kopieren van ${CMAKE_CURRENT_SOURCE_DIR}/Ververs naar ${RUNTIME_OUTPUT_DIRECTORY}/Voorbeeld/Ververs" VERBATIM
)

add_custom_command(TARGET VOORBEELD POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory ${KERN_SOURCE_DIR}/Ververs ${RUNTIME_OUTPUT_DIRECTORY}/Kern/Ververs
					COMMENT "Ververs Kopieren van ${KERN_SOURCE_DIR}/Ververs naar ${RUNTIME_OUTPUT_DIRECTORY}/Kern/Ververs" VERBATIM
)
