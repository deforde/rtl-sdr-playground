docker build -t rtl-sdr-playground-dev .
docker run^
 -it^
 rtl-sdr-playground-dev
rem --mount type=bind,source=C:/Users/dforde/dev/rtl-sdr-playground,target=/home/rtl-sdr-playground^
