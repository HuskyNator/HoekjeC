
file(GLOB EXAMPLE_SOURCE_FILES CONFIGURE_DEPENDS src/*.c)

add_executable(Example ${EXAMPLE_SOURCE_FILES} ${LOGO})

target_link_libraries(Example PUBLIC HoekjeC)

add_custom_command(TARGET Example POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/files" "$<TARGET_FILE_DIR:Example>/files"
	COMMENT "Copying shaders from ${CMAKE_CURRENT_SOURCE_DIR}/files to $<TARGET_FILE_DIR:Example>/files" VERBATIM)
