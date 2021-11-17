#include "plot.h"
#include "proc.h"

#include <errno.h>
#include <float.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

void* plot_ampl_spectrum(void* args)
{
    plot_ampl_spectrum_args* p = (plot_ampl_spectrum_args*)args;
    rtlsdr_dev_t* dev = p->dev;
    uint8_t* buffer = p->buffer;
    size_t buf_len = p->buf_len;
    fft_desc* fft = p->fft;
    bool* do_exit = p->do_exit;

    float* amplitude_spectrum = (float*)malloc(sizeof(float) * fft->len);

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

        for(size_t i = 0; i < fft->len; ++i) {
            amplitude_spectrum[i] = 10 * log10(sqrt(fft->output[i][0] * fft->output[i][0] + fft->output[i][1] * fft->output[i][1]) + DBL_MIN);
        }

        system("clear");
        plot_amplitude_spectrum(amplitude_spectrum, fft->len);

        const struct timespec tim = { .tv_sec = 0, .tv_nsec = 100000000 };
        nanosleep(&tim, NULL);
    }

    free(amplitude_spectrum);

    pthread_exit(NULL);
}

void* send_iq_data(void* args)
{
    send_iq_data_args* thread_args = (send_iq_data_args*)args;
    rtlsdr_dev_t* dev = thread_args->dev;
    uint8_t* buffer = thread_args->buffer;
    size_t buf_len = thread_args->buf_len;
    int socket = thread_args->socket;
    bool* do_exit = thread_args->do_exit;

    while(!(*do_exit)) {
        int n_read = 0;
        int r = rtlsdr_read_sync(dev, buffer, buf_len, &n_read);
        if (r < 0) {
            fprintf(stderr, "Sync read failed.\n");
            break;
        }
        //printf("Sync read succeeded, bytes read: %d.\n", n_read);

        int s = send(socket, buffer, buf_len, 0);
        if(s < 0) {
            fprintf(stderr, "Send failed: %i: %s.\n", errno, strerror(errno));
            break;
        }

        const struct timespec tim = { .tv_sec = 0, .tv_nsec = 500000000 };
        nanosleep(&tim, NULL);
    }

    pthread_exit(NULL);
}
