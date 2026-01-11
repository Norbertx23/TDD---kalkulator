cmake_minimum_required(VERSION 3.16)
project(CalculatorTDD LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include directories
include_directories(include)

# Source files (to be added)
set(SOURCES
    src/Calculator.cpp
    include/Calculator.hpp
    src/MainWindow.cpp
    include/MainWindow.hpp
    src/main.cpp
)

# Executable
add_executable(CalculatorApp MACOSX_BUNDLE ${SOURCES})
target_link_libraries(CalculatorApp PRIVATE Qt6::Widgets)

# Unit Tests
enable_testing()

add_executable(calculator_tests
    tests/TestCalculator.cpp
    src/Calculator.cpp
)
target_link_libraries(calculator_tests 
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(calculator_tests)
