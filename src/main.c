
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>

#include "rtl-sdr.h"

#include "control.h"
#include "plot.h"
#include "fft.h"

#define NUM_SAMPLES 16384
#define IQ_BUF_LEN NUM_SAMPLES * 2 // * 2 for the I and Q components
#define FFT_LEN NUM_SAMPLES

int main()
{
    int r = 0;
    const int dev_index = 0;
    rtlsdr_dev_t* dev = NULL;
    const uint32_t sample_rate_Hz = 2048000;
    const uint32_t centre_frequency_Hz = 94000000;
    uint8_t buffer[IQ_BUF_LEN];
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

    fft_desc fft;
    init_fft(&fft, FFT_LEN);

    for(;;) {
        r = rtlsdr_read_sync(dev, buffer, IQ_BUF_LEN, &n_read);
        if (r < 0) {
            fprintf(stderr, "Sync read failed.\n");
            return EXIT_FAILURE;
        }
        //printf("Sync read succeeded, bytes read: %d.\n", n_read);

        const bool success = execute_fft(&fft, buffer, IQ_BUF_LEN);
        if(!success) {
            return EXIT_FAILURE;
        }

        float amplitude_spectrum[FFT_LEN];
        for(size_t i = 0; i < FFT_LEN; ++i) {
            amplitude_spectrum[i] = 10*log10(sqrt(fft.output[i][0] * fft.output[i][0] + fft.output[i][1] * fft.output[i][1]) + DBL_MIN);
        }

        system("clear");
        plot_amplitude_spectrum(amplitude_spectrum, fft.len);

        const struct timespec tim = { .tv_sec = 0, .tv_nsec = 100000000 };
        nanosleep(&tim, NULL);
    }

    destroy_fft(&fft);

    return EXIT_SUCCESS;
}
