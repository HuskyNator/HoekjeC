cmake_minimum_required(VERSION 3.10.0)

project(HoekjeC_Verzameling)

SET(LOGO ${PROJECT_SOURCE_DIR}/Logo/Logo.rc)

add_subdirectory(HoekjeC)
add_subdirectory(Voorbeeld)

if(CMAKE_PROJECT_NAME STREQUAL HoekjeC_Verzameling)
	include(CTest)
	add_subdirectory(Testen)
endif()
