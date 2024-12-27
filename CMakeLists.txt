cmake_minimum_required(VERSION 3.30)
Project(mdplop)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



include_directories(include/)
add_executable(mdplop
    src/main.cpp
    src/parser.cpp
)