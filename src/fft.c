
#include "fft.h"

void init_fft(fft_desc* fft, size_t len)
{
    fft->len = len;
    fft->output = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fft->len);
    fft->plan = fftw_plan_dft_1d(fft->len, fft->output, fft->output, FFTW_FORWARD, FFTW_ESTIMATE);
}

bool execute_fft(fft_desc* fft, const uint8_t* const iq_buf, size_t num_samples)
{
    if(num_samples < 2 * fft->len) {
        fprintf(stderr, "%s -> Cannot execute fft of length: %ul, on a input IQ data buffer containing only: %ul samples", __func__, fft->len, num_samples);
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

    return true;
}

void destroy_fft(fft_desc* fft)
{
    fftw_destroy_plan(fft->plan);
    fftw_free(fft->output);
}
