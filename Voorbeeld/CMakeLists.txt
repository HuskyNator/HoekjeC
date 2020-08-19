cmake_minimum_required(VERSION 3.10.0)

project(VOORBEELD)

file(GLOB VOORBEELD_BRON *.c *.h)

add_executable(VOORBEELD ${VOORBEELD_BRON} "Logo.coff")

target_link_libraries(VOORBEELD PUBLIC KERN)
