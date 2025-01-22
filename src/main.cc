#include <fstream>
#include <iostream>
#include <uhd/types/tune_request.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <vector>
#include <yaml-cpp/yaml.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <config_file.yaml>" << std::endl;
    return 1;
  }

  // Load the YAML configuration file
  std::string config_file = argv[1];
  YAML::Node config;
  try {
    config = YAML::LoadFile(config_file);
  } catch (const YAML::Exception &e) {
    std::cerr << "Error loading config file: " << e.what() << std::endl;
    return 1;
  }

  // Read USRP parameters from the config file
  double rx_rate = config["rx_rate"].as<double>(1e6);   // Default: 1 MS/s
  double rx_freq = config["rx_freq"].as<double>(915e6); // Default: 915 MHz
  double rx_gain = config["rx_gain"].as<double>(30);    // Default: 30 dB
  double duration_seconds =
      config["duration_seconds"].as<double>(10.0); // Default: 10 seconds
  double start_time_offset =
      config["start_time_offset"].as<double>(0.1); // Default: 0.1 seconds
  std::string cpu_format =
      config["cpu_format"].as<std::string>("fc32"); // Default: fc32
  std::string wire_format =
      config["wire_format"].as<std::string>("sc16"); // Default: sc16

  // Initialize the USRP device
  uhd::device_addr_t dev_addr;
  uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(dev_addr);

  // Apply the parameters to the USRP
  usrp->set_rx_rate(rx_rate);
  usrp->set_rx_freq(uhd::tune_request_t(rx_freq));
  usrp->set_rx_gain(rx_gain);

  std::cout << "USRP initialized successfully with the following settings:\n";
  std::cout << "  RX Rate: " << rx_rate << " S/s\n";
  std::cout << "  RX Frequency: " << rx_freq << " Hz\n";
  std::cout << "  RX Gain: " << rx_gain << " dB\n";

  // Configure the RX streamer
  uhd::stream_args_t stream_args(cpu_format, wire_format);
  uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

  size_t num_samps = rx_stream->get_max_num_samps();
  std::vector<std::complex<float>> buffer(num_samps);

  uhd::rx_metadata_t metadata;
  uhd::time_spec_t start_time = uhd::time_spec_t(start_time_offset);
  usrp->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);

  // Start receiving and process samples
  double elapsed_time = 0.0;
  while (elapsed_time < duration_seconds) {
    size_t num_rx_samps =
        rx_stream->recv(&buffer.front(), buffer.size(), metadata);

    if (metadata.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE) {
      std::cerr << "Error receiving samples: " << metadata.strerror()
                << std::endl;
      break;
    }

    elapsed_time += num_rx_samps / rx_rate;
    std::cout << "First received sample: " << buffer[0] << std::endl;
  }

  usrp->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS);

  return 0;
}

