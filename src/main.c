#include <rtl-sdr.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "control.h"
#include "fft.h"
#include "plot.h"
#include "proc.h"
#include "server.h"
#include "thread.h"

#define NUM_SAMPLES 16384
#define IQ_BUF_LEN (NUM_SAMPLES * 2) // * 2 for the I and Q components
#define FFT_LEN NUM_SAMPLES

bool do_exit = false;

void signal_handler(int signal)
{
    fprintf(stderr, "Signal caught (%i), exiting.\n", signal);
    do_exit = true;
}

void usage()
{
    printf(
        "rtl-sdr-playground\n\n"
        "Use:\trtl-sdr-playground -f freq\n"
        "\t-f centre_frequency\n"
        "\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{
    int opt = 0;
    int r = 0;
    const int dev_index = 0;
    rtlsdr_dev_t* dev = NULL;
    uint32_t sample_rate_Hz = 1020000;
    uint32_t centre_frequency_Hz = 95700000;
    // uint8_t buffer[IQ_BUF_LEN];

    while((opt = getopt(argc, argv, "f:s:h")) != -1) {
        switch(opt) {
        case 'f':
            centre_frequency_Hz = (uint32_t)atol(optarg);
            break;
        case 's':
            sample_rate_Hz = (uint32_t)atol(optarg);
            break;
        case 'h':
        default:
            usage();
            break;
        }
    }

    r = rtlsdr_open(&dev, (uint32_t)dev_index);
    if(r < 0) {
        fprintf(stderr, "Failed to open rtlsdr device #%d.\n", dev_index);
        return EXIT_FAILURE;
    }
    printf("rtlsdr device opened successfully (index #%d).\n", dev_index);

    set_sample_rate(dev, sample_rate_Hz);
    set_centre_frequency(dev, centre_frequency_Hz);
    set_gain_mode_auto(dev);
    reset_buffer(dev);

    const uint16_t port = 50007;
    int socket = accept_connection(port);
    printf("Client connection accepted.\n");

    // fft_desc_t fft = { .len = 0, .output = NULL, .scratch = NULL };
    // init_fft(&fft, FFT_LEN);

    signal(SIGINT, signal_handler);

    // thread_t proc_thread;
    // send_iq_data_args_t thread_args = { .dev = dev, .buffer = buffer, .buf_len = IQ_BUF_LEN, .socket = socket, .do_exit = &do_exit };
    // launch_thread(&proc_thread, send_iq_data, (void*)&thread_args);
    // join_thread(&proc_thread);

    // thread_t proc_thread;
    // plot_ampl_spectrum_args_t thread_args = { .dev = dev, .buffer = buffer, .buf_len = IQ_BUF_LEN, .fft = &fft, .do_exit = &do_exit };
    // launch_thread(&proc_thread, plot_ampl_spectrum, (void*)&thread_args);
    // join_thread(&proc_thread);

    data_read_callback_args_t callback_args = { .dev = dev, .socket = socket, .do_exit = &do_exit };
    r = rtlsdr_read_async(dev, data_read_callback, (void*)&callback_args, 0, IQ_BUF_LEN);
    if(r < 0) {
        fprintf(stderr, "Async data read failed.\n");
        do_exit = true;
    }

    // destroy_fft(&fft);

    rtlsdr_close(dev);

    printf("Exiting.\n");

    return EXIT_SUCCESS;
}
