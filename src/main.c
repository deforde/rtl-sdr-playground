#include "control.h"
#include "fft.h"
#include "plot.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>

#include <rtl-sdr.h>

#define NUM_SAMPLES 16384
#define IQ_BUF_LEN (NUM_SAMPLES * 2) // * 2 for the I and Q components
#define FFT_LEN NUM_SAMPLES

typedef struct thread_args {
    rtlsdr_dev_t* dev;
    uint8_t* buffer;
    size_t buf_len;
    fft_desc* fft;
    bool* do_exit;
} thread_args;

void* data_consume_thread(void* args)
{
    thread_args* thread_args = args;
    rtlsdr_dev_t* dev = thread_args->dev;
    uint8_t* buffer = thread_args->buffer;
    size_t buf_len = thread_args->buf_len;
    fft_desc* fft = thread_args->fft;
    bool* do_exit = thread_args->do_exit;

    while(!(*do_exit)) {
        int n_read = 0;
        int r = rtlsdr_read_sync(dev, buffer, buf_len, &n_read);
        if (r < 0) {
            fprintf(stderr, "Sync read failed.\n");
            break;
        }
        //printf("Sync read succeeded, bytes read: %d.\n", n_read);

        const bool success = execute_fft(fft, buffer, buf_len);
        if(!success) {
            break;
        }

        float amplitude_spectrum[FFT_LEN];
        for(size_t i = 0; i < FFT_LEN; ++i) {
            amplitude_spectrum[i] = 10*log10(sqrt(fft->output[i][0] * fft->output[i][0] + fft->output[i][1] * fft->output[i][1]) + DBL_MIN);
        }

        system("clear");
        plot_amplitude_spectrum(amplitude_spectrum, fft->len);

        const struct timespec tim = { .tv_sec = 0, .tv_nsec = 100000000 };
        nanosleep(&tim, NULL);
    }

    pthread_exit(NULL);
}

int main()
{
    int r = 0;
    const int dev_index = 0;
    rtlsdr_dev_t* dev = NULL;
    const uint32_t sample_rate_Hz = 2048000;
    const uint32_t centre_frequency_Hz = 94000000;
    uint8_t buffer[IQ_BUF_LEN];
    bool do_exit = false;

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

    fft_desc fft = { .len = 0, .output = NULL, .scratch = NULL };
    init_fft(&fft, FFT_LEN);

    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
    pthread_t thread_handle;

    thread_args thread_args = { .dev = dev, .buffer = buffer, .buf_len = IQ_BUF_LEN, .fft = &fft, .do_exit = &do_exit };
    pthread_create(&thread_handle, &thread_attr, data_consume_thread, (void*)&thread_args);

    pthread_attr_destroy(&thread_attr);

    void* thread_status;
    pthread_join(thread_handle, &thread_status);

    destroy_fft(&fft);

    return EXIT_SUCCESS;
}
