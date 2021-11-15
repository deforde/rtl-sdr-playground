#ifndef FFT_H
#define FFT_H

#include <stdlib.h>
#include <stdint.h>

#include <fftw3.h>

size_t fft(const uint8_t* const buf, size_t len, fftw_complex** p_fft_data_buf);

#endif
