cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
project("OpenCV2-Test")

include(build/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB SRC_FILES *.h *.cpp *.hpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})