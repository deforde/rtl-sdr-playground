
#include <stdlib.h>
#include <stdio.h>

#include "rtl-sdr.h"

#include "control.h"
#include "plot.h"

int main()
{
    int r = 0;
    const int dev_index = 0;
    rtlsdr_dev_t* dev = NULL;
    const uint32_t sample_rate_Hz = 2048000;
    const uint32_t centre_frequency_Hz = 94500000;
    size_t buf_len = 4096;
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

    r = rtlsdr_read_sync(dev, buffer, buf_len, &n_read);
    if (r < 0) {
        fprintf(stderr, "Sync read failed.\n");
        return EXIT_FAILURE;
    }
    printf("Sync read succeeded, bytes read: %d.\n", n_read);

    return EXIT_SUCCESS;
}
