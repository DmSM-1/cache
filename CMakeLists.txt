cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

include_directories("include")

project(lfu_cache)
set(lfu_files src/LFU_main.cpp)
add_executable(lfu_cache ${lfu_files})

project(prf_cache)
set(prf_files src/Perfect_main.cpp)
add_executable(prf_cache ${prf_files})