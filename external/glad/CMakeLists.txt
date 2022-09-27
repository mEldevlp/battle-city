cmake_minimum_required(VERSION 3.24.2 FATAL_ERROR)

project(glad)

add_library(glad STATIC include/glad/glad.h src/glad.c)

target_include_directories(glad PUBLIC include/)