cmake_minimum_required(VERSION 3.16)
project(QTradeStation VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/dashboardwidget.h
    src/dashboardwidget.cpp
    src/tradingcardwidget.h
    src/tradingcardwidget.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
