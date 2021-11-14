cmake_minimum_required(VERSION 3.0.0)
project(rtl-sdr-playground C)

add_executable(rtl-sdr-playground main.c)

target_link_libraries(rtl-sdr-playground rtlsdr pthread)
