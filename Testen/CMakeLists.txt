cmake_minimum_required(VERSION 3.10.0)

project(HoekjeC_Testen)

add_executable(test_lijsten test_lijsten.c)
target_link_libraries(test_lijsten PUBLIC -static HoekjeC)

# Zie https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html#properties-on-tests

add_test(lege_test test_lijsten)
add_test(foute_test test_lijsten .)
set_tests_properties(lege_test foute_test PROPERTIES WILL_FAIL TRUE)

add_test(lijst_test test_lijsten L)

add_test(schakellijst_test test_lijsten s)

# add_test(sleutellijst_test test_lijsten S)
add_test(sleutellijst_test test_lijsten S)
