cmake_minimum_required(VERSION 3.16)
project(practice1 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Test REQUIRED)

add_executable(practice1 main.cpp mainwindow.cpp)
target_link_libraries(practice1 PRIVATE Qt6::Widgets)

add_executable(TestApp test_main.cpp mainwindow.cpp
)

target_link_libraries(TestApp PRIVATE Qt6::Widgets Qt6::Test)