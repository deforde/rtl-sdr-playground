
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>

#include "rtl-sdr.h"

#include "control.h"
#include "plot.h"
#include "fft.h"

int main()
{
    int r = 0;
    const int dev_index = 0;
    rtlsdr_dev_t* dev = NULL;
    const uint32_t sample_rate_Hz = 2048000;
    const uint32_t centre_frequency_Hz = 94500000;
    const size_t num_samples = 4096;
    size_t buf_len = num_samples * 2; // * 2 for the I and Q components
    uint8_t buffer[buf_len];
    int n_read = 0;

    r = rtlsdr_open(&dev, (uint32_t)dev_index);
    if (r < 0) {
        fprintf(stderr, "Failed to open rtlsdr device #%d.\n", dev_index);
        return EXIT_FAILURE;
    }
    printf("rtlsdr device opened successfully (index #%d).\n", dev_index);

    set_sample_rate(dev, sample_rate_Hz);
    set_centre_frequency(dev, centre_frequency_Hz);
    set_gain_mode_auto(dev);
    reset_buffer(dev);

    for(;;) {
        r = rtlsdr_read_sync(dev, buffer, buf_len, &n_read);
        if (r < 0) {
            fprintf(stderr, "Sync read failed.\n");
            return EXIT_FAILURE;
        }
        //printf("Sync read succeeded, bytes read: %d.\n", n_read);

        fftw_complex** p_fft_data_buf;
        size_t fft_len = fft(buffer, buf_len, p_fft_data_buf);
        fftw_complex* fft_data_buf = *p_fft_data_buf;

        float amplitude_spectrum[fft_len];
        for(size_t i = 0; i < fft_len; ++i) {
            amplitude_spectrum[i] = 10*log10(sqrt(fft_data_buf[i][0] * fft_data_buf[i][0] + fft_data_buf[i][1] * fft_data_buf[i][1]) + DBL_MIN);
        }
        fftw_free(fft_data_buf);

        system("clear");
        plot_amplitude_spectrum(amplitude_spectrum, fft_len);

        const struct timespec tim = { .tv_sec = 0, .tv_nsec = 100000000 };
        nanosleep(&tim, NULL);
    }

    return EXIT_SUCCESS;
}
