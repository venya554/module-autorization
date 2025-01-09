cmake_minimum_required(VERSION 3.10)
project(BackendServer)

# Находим библиотеку cpprestsdk
find_package(cpprestsdk REQUIRED)

# Создаем исполняемый файл
add_executable(BackendServer main.cpp)

# Подключаем библиотеку cpprestsdk
target_link_libraries(BackendServer PRIVATE cpprestsdk::cpprestsdk) 