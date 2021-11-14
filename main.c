
#include <stdlib.h>
#include <stdio.h>

#include "rtl-sdr.h"

int main()
{
    int r = 0;
    int dev_index = 0;
    rtlsdr_dev_t* dev = NULL;

    r = rtlsdr_open(&dev, (uint32_t)dev_index);
    if (r < 0) {
        fprintf(stderr, "Failed to open rtlsdr device #%d.\n", dev_index);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "rtlsdr device opened successfully (index #%d).\n", dev_index);

    return EXIT_SUCCESS;
}
