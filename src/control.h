#ifndef CONTROL_H
#define CONTROL_H

#include <rtl-sdr.h>

#include <stdlib.h>

int set_sample_rate(rtlsdr_dev_t* dev, uint32_t sample_rate_Hz);
int set_centre_frequency(rtlsdr_dev_t* dev, uint32_t frequency_Hz);
int set_gain_mode_auto(rtlsdr_dev_t* dev);
int set_gain(rtlsdr_dev_t *dev, int gain);
int reset_buffer(rtlsdr_dev_t* dev);

#endif //CONTROL_H
