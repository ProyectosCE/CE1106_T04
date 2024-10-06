cmake_minimum_required(VERSION 3.29)
project(Client)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

#FIND PACKEGES HERE:
find_package(cJSON)

#Executables here:
add_executable(Client main.c)


#Targets here:
target_link_libraries(Client cjson::cjson)