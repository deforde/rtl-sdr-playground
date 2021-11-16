import socket
from math import log10
from sys import float_info

import matplotlib.pyplot as plt
from numpy import fft, linspace

host = '192.168.7.2' # TODO: Should be set programatically rather than hard-coded
port = 50007
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((host, port))

    rx_bytes = s.recv(32768)
    rx_data = list(rx_bytes)

    i_data = rx_data[::2]
    q_data = rx_data[1::2]

    iq_data = [complex((x - 127) / 127, (y - 127) / 127) for x,y in zip(i_data, q_data)]

    fft_data = fft.fftshift(fft.fft(iq_data))

    freq = linspace(-0.5, 0.5, len(fft_data))
    ampl_data = [10*log10(abs(v) / len(fft_data) + float_info.epsilon) for v in fft_data]

    plt.plot(freq, ampl_data)
    plt.show()
