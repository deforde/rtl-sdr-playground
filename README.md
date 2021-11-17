# rtl-sdr-playground

A playground for rtl-sdr related software.

### Server Side
- Written in `C`.
- Built using the `CMakeLists.txt` file in the root directory.
- Designed to run on a Unix platform to which a USB RTL-SDR device is connected.
- Opens a handle to the RTL-SDR device, and configures it for data capture (centre frequency, sample rate, gain etc.).
- Opens a TCP server socket and accepts a single client connection.
- Captures blocks of 8bit IQ sample data from the device and transmits it to the client.

### Client Side
- Written in `Python`.
- Source located here: `src/client/client.py`
- Connects to the server.
- Continuously reads IQ sample data blocks from the server and dynamically plots the amplitude spectrum thereof.

----------------
### Dependencies

- [rtl-sdr](https://github.com/osmocom/rtl-sdr)
- [fftw](https://www.fftw.org/)
- [Python v3.8](https://www.python.org/downloads/)
- [Matplotlib](https://matplotlib.org/stable/index.html)
- [Numpy](https://numpy.org/)
