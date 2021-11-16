
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

float peak_hold(float* ampl, size_t len)
{
    float peak = -FLT_MAX;
    for(size_t i = 0; i < len; ++i) {
        peak = fmax(peak, ampl[i]);
    }
    return peak;
}

float mean(float* ampl, size_t len)
{
    float peak = 0.0f;
    for(size_t i = 0; i < len; ++i) {
        peak += ampl[i];
    }
    return peak / len;
}

void plot_amplitude_spectrum(float* ampl, size_t len)
{
    static const size_t display_height = 46;
    static const size_t display_width = 202;

    if(display_width > len) {
        //TODO
        fprintf(stderr, "Implementation for case in which the source data len (%lu) is less than the display width (%lu) does not yet exist!", len, display_width);
        return;
    }

    const double resample_ratio = (double)len / display_width;

    float resampled_data[display_width];
    for(size_t i = 0; i < display_width; ++i) {
        const size_t input_start_index = fmin(floor(i * resample_ratio), len - 1);
        const size_t input_end_index = fmin(ceil((i + 1) * resample_ratio), len);
        const size_t input_block_len = input_end_index - input_start_index;

        resampled_data[i] = peak_hold(&ampl[input_start_index], input_block_len);
    }

    static float min_ampl = FLT_MAX;
    static float max_ampl = -FLT_MAX;

    const float data_min_ampl = get_min_ampl(resampled_data, display_width);
    const float data_max_ampl = get_max_ampl(resampled_data, display_width);

    min_ampl = fminf(min_ampl, data_min_ampl);
    max_ampl = fmaxf(max_ampl, data_max_ampl);

    const float ampl_range = max_ampl - min_ampl;

    uint32_t discretised_data[display_width];
    for(size_t i = 0; i < display_width; ++i) {
        discretised_data[i] = fmin(round(((resampled_data[i] - min_ampl) / ampl_range) * display_height), display_height - 1);
    }

    const size_t rows = display_height;
    const size_t y_axis_label_len = 7;
    const size_t row_trailer_len = 1; //for \n
    const size_t columns = y_axis_label_len + display_width + row_trailer_len;
    char display_grid[rows * columns + 1]; // + 1 for trailing \0
    float discretized_level = max_ampl;
    float discretized_ampl_res = ampl_range / display_height;
    for(size_t i = 0; i < rows; ++i) {
        int bytes_written = snprintf(&display_grid[i * columns], y_axis_label_len, "%+.2f", discretized_level);
        discretized_level -= discretized_ampl_res;
        memset(&display_grid[i * columns + bytes_written - 1], ' ', y_axis_label_len - bytes_written + 1);
        memset(&display_grid[i * columns + y_axis_label_len], ' ', columns - row_trailer_len - y_axis_label_len);
        display_grid[i * columns + columns - 1] = '\n';
    }
    display_grid[rows * columns] = 0;
    for(size_t i = 0; i < display_width; ++i) {
        const uint32_t ampl = discretised_data[i];
        const size_t row_index = display_height - 1 - ampl;
        display_grid[row_index * columns + i + y_axis_label_len] = '-';
    }
    printf("%s", display_grid);
}
