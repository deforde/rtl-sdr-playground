FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Africa/Johannesburg

RUN apt update && apt install -y \
 gcc-8-arm-linux-gnueabihf \
 git \
 cmake
# \
# && rm -rf /var/lib/apt/lists/*

WORKDIR /home/
RUN git clone https://github.com/deforde/rtl-sdr-playground
