# UU-Agent

UU-Agent is an open source program primarily built for executing, assisting, and testing various attacks on wireless User to User Interface. This project provides a CLI-based interface to collect IQ samples from UHD-compatible devices and ZeroMQ sources. The agent collects IQ data over specified RF parameters and saves it to binary files. You can configure parameters using the config files in the `configs/` directory.

## Building the UU-Agent (Linux CLI)

Follow the steps below to build and run the UU-Agent on a Linux-based command line interface.

### 1. Clone the Repository

```bash
git clone https://github.com/oran-testing/uu-agent.git
cd uu-agent
```

### 2. Build the Project

```bash
mkdir build
cd build
cmake ..
make -j
```

*Note: Ensure that cmake and make are installed on your system. You may also need UHD libraries, ZeroMQ libraries, and other dependencies.*

## Running the UU-Agent

After successfully building the project, you can run the UU-Agent using a configuration file or command line options.

### Using Configuration File
```bash
# For UHD devices
./uuagent/uuagent --config ../configs/uhd_collection.conf

# For ZeroMQ sources
./uuagent/uuagent --config ../configs/zmq_collection.conf
```

### Using Command Line Options
```bash
# Available command line options
./uuagent/uuagent --help

# Basic UHD usage
./uuagent/uuagent --rf.type uhd --rf.rx_freq 2400000000 --rf.srate 1000000

# ZeroMQ usage
./uuagent/uuagent --rf.type zmq --rf.device_args "tcp://localhost:5555"

# Custom parameters
./uuagent/uuagent --rf.type uhd --rf.rx_freq 915000000 --rf.rx_gain 30 --rf.num_samples 10000000 --rf.iq_file my_data.bin
```

## Configuration

- The UU-Agent uses configuration files to define parameters such as RF frequency, sample rate, gain, etc.
- You can modify the parameters in `configs/uhd_collection.conf` for UHD devices or `configs/zmq_collection.conf` for ZeroMQ sources to suit your specific use case.

## Requirements

- Linux-based OS
- CMake (version 3.10 or higher recommended)
- Make
- C++ compiler (GCC or Clang)
- UHD library
- ZeroMQ library
- Boost program_options
- yaml-cpp

To install the necessary tools on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential cmake libuhd-dev libzmq3-dev libboost-program-options-dev libyaml-cpp-dev pkg-config
```
