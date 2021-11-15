
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plot.h"

float get_min_ampl(float* ampl, size_t len)
{
    float min_ampl = FLT_MAX;
    for(size_t i = 0; i < len; ++i) {
        min_ampl = fmin(min_ampl, ampl[i]);
    }
    return min_ampl;
}

float get_max_ampl(float* ampl, size_t len)
{
    float max_ampl = -FLT_MAX;
    for(size_t i = 0; i < len; ++i) {
        max_ampl = fmax(max_ampl, ampl[i]);
    }
    return max_ampl;
}

void plot_amplitude_spectrum(float* ampl, size_t len)
{
    const size_t display_height = 46;
    const size_t display_width = 202;

    const float target_min_ampl = -40;
    const float target_max_ampl = 0;

    const float data_min_ampl = get_min_ampl(ampl, len);
    const float data_max_ampl = get_max_ampl(ampl, len);

    const float min_ampl = fmin(data_min_ampl, target_min_ampl);
    const float max_ampl = fmax(data_max_ampl, target_max_ampl);

    const float ampl_range = max_ampl - min_ampl;

    uint32_t resampled_data[display_width];
    if(display_width <= len) {
        const double resample_ratio = (double)len / display_width;
        for(size_t i = 0; i < display_width; ++i) {
            const size_t input_start_index = fmin(floor(i * resample_ratio), len - 1);
            const size_t input_end_index = fmin(ceil((i + 1) * resample_ratio), len);

            // float input_block_max = min_ampl;
            // for(size_t j = input_start_index; j < input_end_index; ++j) {
            //     input_block_max = fmax(input_block_max, ampl[j]);
            // }
            float input_block_mean = 0.0f;
            for(size_t j = input_start_index; j < input_end_index; ++j) {
                input_block_mean += ampl[j];
            }
            input_block_mean /= (input_end_index - input_start_index);

            const uint32_t discretised_ampl = fmin(roundf(((input_block_mean - min_ampl) / ampl_range) * display_height), display_height - 1);
            resampled_data[i] = discretised_ampl;
        }
    }
    else {
        //TODO
        fprintf(stderr, "Implementation for case in which the source data len (%lu) is less than the display width (%lu) does not yet exist!", len, display_width);
        return;
    }

    const size_t rows = display_height;
    const size_t y_axis_label_len = 7;
    const size_t trailer_len = 2; //for \n and \0
    const size_t columns = y_axis_label_len + display_width + trailer_len;
    char display_grid[rows][columns];
    float discretized_level = max_ampl;
    float discretized_ampl_res = ampl_range / display_height;
    for(size_t i = 0; i < rows; ++i) {
        int bytes_written = snprintf(display_grid[i], 7, "%+.2f", discretized_level);
        discretized_level -= discretized_ampl_res;
        memset(&display_grid[i][bytes_written - 1], ' ', y_axis_label_len - bytes_written + 1);
        memset(&display_grid[i][y_axis_label_len], ' ', columns - trailer_len - y_axis_label_len);
        display_grid[i][columns - 2] = '\n';
        display_grid[i][columns - 1] = 0;
    }
    for(size_t i = 0; i < display_width; ++i) {
        const uint32_t ampl = resampled_data[i];
        const size_t row_index = display_height - 1 - ampl;
        display_grid[row_index][i + y_axis_label_len] = '-';
    }
    for(size_t i = 0; i < rows; ++i) {
        printf("%s", display_grid[i]);
    }
}
