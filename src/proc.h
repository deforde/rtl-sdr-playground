#ifndef PROC_H
#define PROC_H

#include <rtl-sdr.h>
#include <stdbool.h>
#include <stdint.h>

#include "fft.h"

typedef struct {
    rtlsdr_dev_t* dev;
    uint8_t* buffer;
    size_t buf_len;
    fft_desc_t* fft;
    bool* do_exit;
} plot_ampl_spectrum_args_t;

void* plot_ampl_spectrum(void* args);

typedef struct {
    rtlsdr_dev_t* dev;
    uint8_t* buffer;
    size_t buf_len;
    int socket;
    bool* do_exit;
} send_iq_data_args_t;

void* send_iq_data(void* args);

typedef struct {
    rtlsdr_dev_t* dev;
    int socket;
    bool* do_exit;
} data_read_callback_args_t;

void data_read_callback(unsigned char* buffer, uint32_t len, void* args);

#endif //PROC_H
