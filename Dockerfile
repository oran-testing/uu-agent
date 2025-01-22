ARG OS_VERSION=24.04
ARG LIB=uhd
ARG LIB_VERSION=4.7.0.0
ARG MARCH=native
ARG NUM_CORES=""

FROM ubuntu:$OS_VERSION

ENV CONFIG="configs/basic_listener.yaml"
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y \
    cmake \
    make \
    gcc \
    g++ \
    uhd-host \
    libuhd-dev \
    libyaml-cpp-dev


COPY . /app

WORKDIR /app

RUN mkdir -p build && rm -rf build/*

WORKDIR /app/build

RUN cmake ../ && \
    make -j$(nproc) && \
    make install

WORKDIR /app

CMD [ "sh", "-c", "/usr/local/bin/uuagent \"${CONFIG}\"" ]
