
#include "fft.h"

size_t fft(const uint8_t* const buf, size_t len, fftw_complex** p_fft_data_buf)
{
    size_t fft_len = len / 2;

    fftw_complex* fft_data_buf;
    fftw_plan plan;

    fft_data_buf = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fft_len);
    *p_fft_data_buf = fft_data_buf;

    plan = fftw_plan_dft_1d(fft_len, fft_data_buf, fft_data_buf, FFTW_FORWARD, FFTW_ESTIMATE);

    for(size_t i = 0; i < fft_len; ++i) {
        fft_data_buf[i][0] = ((double)buf[2*i] - INT8_MAX) / INT8_MAX;
        fft_data_buf[i][1] = ((double)buf[2*i+1] - INT8_MAX) / INT8_MAX;
    }

    fftw_execute(plan);

    for(size_t i = 0; i < fft_len; ++i) {
        fft_data_buf[i][0] /= fft_len;
        fft_data_buf[i][1] /= fft_len;
    }

    fftw_destroy_plan(plan);

    return fft_len;
}
