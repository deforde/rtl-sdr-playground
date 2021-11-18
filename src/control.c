#include "control.h"

#include <stdio.h>

int set_sample_rate(rtlsdr_dev_t* dev, uint32_t sample_rate_Hz)
{
    int r = rtlsdr_set_sample_rate(dev, sample_rate_Hz);
    if (r < 0) {
        fprintf(stderr, "Failed to set sample rate.\n");
    } else {
        printf("Sample rate set to: %u Hz.\n", sample_rate_Hz);
    }
    return r;
}

int set_centre_frequency(rtlsdr_dev_t* dev, uint32_t centre_frequency_Hz)
{
    int r = rtlsdr_set_center_freq(dev, centre_frequency_Hz);
    if (r < 0) {
        fprintf(stderr, "Failed to set centre frequency.\n");
    } else {
        printf("Centre frequency set to: %u Hz.\n", centre_frequency_Hz);
    }
    return r;
}

int set_gain_mode_auto(rtlsdr_dev_t* dev)
{
    int r = rtlsdr_set_tuner_gain_mode(dev, 0);
    if (r != 0) {
        fprintf(stderr, "Failed to set tuner gain mode to auto.\n");
    } else {
        printf("Set tuner gain mode to auto.\n");
    }
    return r;
}

int set_gain(rtlsdr_dev_t *dev, int gain)
{
    int r = rtlsdr_set_tuner_gain_mode(dev, 1);
    if (r < 0) {
        fprintf(stderr, "Failed to set tuner gain mode to manual.\n");
        return r;
    }
    printf("Set tuner gain mode to manual.\n");

    r = rtlsdr_set_tuner_gain(dev, gain);
    if (r != 0) {
        fprintf(stderr, "Failed to set tuner gain.\n");
    }
    printf("Tuner gain set to %.02f dB.\n", gain/10.0);
    return r;
}

int reset_buffer(rtlsdr_dev_t* dev)
{
    int r = rtlsdr_reset_buffer(dev);
    if (r < 0) {
        fprintf(stderr, "Failed to reset buffer.\n");
    } else {
        printf("Buffer reset successfully.\n");
    }
    return r;
}
