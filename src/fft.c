#include "fft.h"

#include <string.h>

void init_fft(fft_desc_t* fft, size_t len)
{
    fft->len = len;
    fft->output = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fft->len);
    fft->plan = fftw_plan_dft_1d(fft->len, fft->output, fft->output, FFTW_FORWARD, FFTW_ESTIMATE);
    fft->scratch = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fft->len / 2);
}

bool execute_fft(fft_desc_t* fft, const uint8_t* const iq_buf, size_t num_samples)
{
    if(num_samples < 2 * fft->len) {
        fprintf(stderr, "Cannot execute fft of length: %ul, on a input IQ data buffer containing only: %ul samples.\n", fft->len, num_samples);
        return false;
    }

    for(size_t i = 0; i < fft->len; ++i) {
        fft->output[i][0] = ((double)iq_buf[2*i] - INT8_MAX) / INT8_MAX;
        fft->output[i][1] = ((double)iq_buf[2*i+1] - INT8_MAX) / INT8_MAX;
    }

    fftw_execute(fft->plan);

    for(size_t i = 0; i < fft->len; ++i) {
        fft->output[i][0] /= fft->len;
        fft->output[i][1] /= fft->len;
    }

    memcpy(fft->scratch, fft->output, sizeof(fftw_complex) * fft->len / 2);
    memcpy(fft->output, &fft->output[fft->len / 2], sizeof(fftw_complex) * fft->len / 2);
    memcpy(&fft->output[fft->len / 2], fft->scratch, sizeof(fftw_complex) * fft->len / 2);

    return true;
}

void destroy_fft(fft_desc_t* fft)
{
    fftw_destroy_plan(fft->plan);
    fftw_free(fft->output);
}
