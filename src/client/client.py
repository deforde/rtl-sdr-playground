import socket
from math import log10
from sys import float_info
import time

import matplotlib.pyplot as plt
from numpy import fft, linspace

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    host = '192.168.7.2' # TODO: Should be set programatically rather than hard-coded
    port = 50007

    s.connect((host, port))

    num_iq_samples = 32768
    fft_len = num_iq_samples // 2
    freq_axis = linspace(-0.5, 0.5, fft_len)

    ampl_data = [0.0 for i in range(fft_len)]

    plt.style.use('ggplot')
    plt.ion()
    fig = plt.figure()
    ax = fig.add_subplot(111)
    spectrum, = ax.plot(freq_axis, ampl_data)
    plt.ylabel('Amplitude (dB)')
    plt.xlabel('Frequency (normalised)')
    plt.title('RTL-SDR Spectrum')

    min_ampl = min(ampl_data)
    max_ampl = max(ampl_data)

    last_plot_time_ns = 0

    while True:
        rx_bytes = s.recv(num_iq_samples)

        now_ns = time.time_ns()
        delta_ns = now_ns - last_plot_time_ns
        if delta_ns < 500_000_000:
            continue

        last_plot_time_ns = now_ns

        rx_data = list(rx_bytes)

        i_data = rx_data[::2]
        q_data = rx_data[1::2]

        iq_data = [complex((x - 127) / 127, (y - 127) / 127) for x,y in zip(i_data, q_data)]

        fft_data = fft.fftshift(fft.fft(iq_data))

        ampl_data = [10 * log10(abs(v) / len(fft_data) + float_info.epsilon) for v in fft_data]

        local_min = min(ampl_data)
        local_max = max(ampl_data)
        if local_min < min_ampl or local_max > max_ampl:
            min_ampl = min(local_min, min_ampl)
            max_ampl = max(local_max, max_ampl)
            plt.ylim([min_ampl, max_ampl])

        spectrum.set_ydata(ampl_data)
        plt.pause(0.1)
