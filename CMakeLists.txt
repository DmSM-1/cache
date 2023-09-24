cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(lfu_cache)
set(lfu_files src/LFU_main.cpp include/LFU_Cache.hpp)
add_executable(lfu_cache ${lfu_files})

project(prf_cache)
set(prf_files src/Perfect_main.cpp include/Perfect_Cache.hpp)
add_executable(prf_cache ${prf_files})