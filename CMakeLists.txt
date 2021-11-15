cmake_minimum_required(VERSION 3.0.0)
project(rtl-sdr-playground C)

add_executable(rtl-sdr-playground src/main.c src/plot.c src/control.c src/fft.c)

target_link_libraries(rtl-sdr-playground rtlsdr fftw3 pthread m)
