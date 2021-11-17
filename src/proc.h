#ifndef PROC_H
#define PROC_H

#include "fft.h"

#include <rtl-sdr.h>

#include <stdbool.h>
#include <stdint.h>

typedef struct plot_ampl_spectrum_args {
    rtlsdr_dev_t* dev;
    uint8_t* buffer;
    size_t buf_len;
    fft_desc* fft;
    bool* do_exit;
} plot_ampl_spectrum_args;

void* plot_ampl_spectrum(void* args);

typedef struct send_iq_data_args {
    rtlsdr_dev_t* dev;
    uint8_t* buffer;
    size_t buf_len;
    int socket;
    bool* do_exit;
} send_iq_data_args;

void* send_iq_data(void* args);

typedef struct data_read_callback_args {
    rtlsdr_dev_t* dev;
    int socket;
    bool* do_exit;
} data_read_callback_args;

void data_read_callback(unsigned char* buffer, uint32_t len, void* args);

#endif //PROC_H
