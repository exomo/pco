cmake_minimum_required (VERSION 3.12)
cmake_policy(SET CMP0074 NEW)
project(PCO)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

set(CXX_STANDARD 17)

add_library(pco PostCreationObjects.cpp application.cpp window.cpp)
target_link_libraries(pco PUBLIC gdi32 user32 kernel32 comctl32 uxtheme)
## Enable the following line for unicode build
# target_link_options(pco PUBLIC -municode)

add_executable(PCOtest pcotest.cpp pcotest.rc)
target_link_libraries(PCOtest pco )
