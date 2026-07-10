cmake_minimum_required(VERSION 3.5)

project(practice2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматически ищет Qt6 или Qt5 в зависимости от того, что установлено в системе
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

# ==========================================
# 1. СБОРКА ОСНОВНОЙ ИГРЫ (practice2)
# ==========================================
add_executable(practice2
    main.cpp
    mainwindow.cpp
    mainwindow.h
    gameboard.cpp
    gameboard.h
    puzzlepiece.h
)

target_link_libraries(practice2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# ==========================================
# 2. СБОРКА ЮНИТ-ТЕСТОВ (puzzle_tests)
# ==========================================
add_executable(puzzle_tests
    tst_puzzle.cpp
    gameboard.cpp
    gameboard.h
    puzzlepiece.h
)

target_link_libraries(puzzle_tests PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)