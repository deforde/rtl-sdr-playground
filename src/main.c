#include "control.h"
#include "fft.h"
#include "plot.h"
#include "proc.h"
#include "server.h"

#include <rtl-sdr.h>

#include <errno.h>
#include <float.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

#define NUM_SAMPLES 16384
#define IQ_BUF_LEN (NUM_SAMPLES * 2) // * 2 for the I and Q components
#define FFT_LEN NUM_SAMPLES

bool do_exit = false;

void signal_handler(int signal)
{
    fprintf(stderr, "Signal caught (%i), exiting.\n", signal);
    do_exit = true;
}

int main()
{
    int r = 0;
    const int dev_index = 0;
    rtlsdr_dev_t* dev = NULL;
    const uint32_t sample_rate_Hz = 2048000;
    const uint32_t centre_frequency_Hz = 94000000;
    uint8_t buffer[IQ_BUF_LEN];
    //const uint16_t port = 50007;

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

    // int socket = accept_connection(port);
    // printf("Client connection accepted.\n");

    fft_desc fft = { .len = 0, .output = NULL, .scratch = NULL };
    init_fft(&fft, FFT_LEN);

    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
    pthread_t thread_handle;

    signal(SIGINT, signal_handler);

    // send_iq_data_args thread_args = { .dev = dev, .buffer = buffer, .buf_len = IQ_BUF_LEN, .socket = socket, .do_exit = &do_exit };
    // pthread_create(&thread_handle, &thread_attr, send_iq_data, (void*)&thread_args);

    plot_ampl_spectrum_args thread_args = { .dev = dev, .buffer = buffer, .buf_len = IQ_BUF_LEN, .fft = &fft, .do_exit = &do_exit };
    pthread_create(&thread_handle, &thread_attr, plot_ampl_spectrum, (void*)&thread_args);

    pthread_attr_destroy(&thread_attr);

    void* thread_status;
    pthread_join(thread_handle, &thread_status);

    destroy_fft(&fft);

    printf("Exiting.\n");

    return EXIT_SUCCESS;
}
