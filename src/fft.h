#ifndef FFT_H
#define FFT_H

#include <fftw3.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct fft_desc
{
    size_t len;
    fftw_complex* output;
    fftw_plan plan;
    fftw_complex* scratch;
} fft_desc;

void init_fft(fft_desc* fft, size_t len);

bool execute_fft(fft_desc* fft, const uint8_t* iq_buf, size_t num_samples);

void destroy_fft(fft_desc* fft);

#endif //FFT_H
