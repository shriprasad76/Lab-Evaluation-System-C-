cmake_minimum_required(VERSION 3.16)
project(LabEvaluationSystem LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(LabEvaluationSystem
    main.cpp
    mainwindow.cpp
    student.cpp
)

target_link_libraries(LabEvaluationSystem PRIVATE Qt6::Widgets)

target_include_directories(LabEvaluationSystem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
