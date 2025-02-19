#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/program_options.hpp>
#include <complex>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <uhd/stream.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <vector>

#include "args.h"

namespace bpo = boost::program_options;

// Parse command-line arguments
void parse_args(all_args_t &args, int argc, char *argv[]) {
  bpo::options_description general("General options");
  general.add_options()("help,h", "Produce help message")(
      "version,v", "Print version information and exit");

  bpo::options_description common("Configuration options");
  common.add_options()(
      "rf.type",
      bpo::value<std::string>(&args.rf.rf_type)->default_value("uhd"),
      "RF hardware type")(
      "rf.srate", bpo::value<double>(&args.rf.srate_hz)->default_value(1e6),
      "Sampling rate in Hz")(
      "rf.rx_gain", bpo::value<float>(&args.rf.rx_gain)->default_value(30),
      "Receiver gain (dB)")(
      "rf.tx_gain", bpo::value<float>(&args.rf.tx_gain)->default_value(0),
      "Transmitter gain (dB)")(
      "rf.rx_freq", bpo::value<float>(&args.rf.rx_freq)->default_value(2.4e9),
      "RX frequency in Hz")(
      "rf.tx_freq", bpo::value<float>(&args.rf.tx_freq)->default_value(2.4e9),
      "TX frequency in Hz")(
      "rf.num_samples",
      bpo::value<size_t>(&args.rf.num_samples)->default_value(1000000),
      "Number of samples to collect")(
      "rf.iq_file",
      bpo::value<std::string>(&args.rf.output_file)
          ->default_value("iq_data.bin"),
      "Output binary file")(
      "rf.device_args",
      bpo::value<std::string>(&args.rf.device_args)->default_value(""),
      "Output binary file");

  bpo::options_description all_options;
  all_options.add(general).add(common);

  bpo::variables_map vm;
  bpo::store(bpo::parse_command_line(argc, argv, all_options), vm);
  bpo::notify(vm);

  if (vm.count("help")) {
    std::cout << all_options << std::endl;
    exit(0);
  }
}

// Collect IQ data using UHD
void collect_iq_data(const all_args_t &args) {
  // Create a USRP device
  uhd::usrp::multi_usrp::sptr usrp =
      uhd::usrp::multi_usrp::make(args.rf.device_args);

  // Configure the USRP
  usrp->set_rx_rate(args.rf.srate_hz);
  usrp->set_rx_freq(uhd::tune_request_t(args.rf.rx_freq));
  usrp->set_rx_gain(args.rf.rx_gain);

  // Set up the RX streamer
  uhd::stream_args_t stream_args("fc32",
                                 "sc16"); // Floating-point complex format
  uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

  // Buffer for IQ data
  std::vector<std::complex<float>> buffer(args.rf.num_samples);
  uhd::rx_metadata_t md;

  // Start streaming
  std::cout << "Receiving " << args.rf.num_samples << " samples...\n";
  uhd::stream_cmd_t stream_cmd(
      uhd::stream_cmd_t::STREAM_MODE_NUM_SAMPS_AND_DONE);
  stream_cmd.num_samps = args.rf.num_samples;
  stream_cmd.stream_now = true;
  rx_stream->issue_stream_cmd(stream_cmd);

  // Receive samples
  size_t received_samples =
      rx_stream->recv(buffer.data(), args.rf.num_samples, md);

  // Check for errors
  if (md.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE) {
    std::cerr << "Error receiving samples: " << md.strerror() << std::endl;
    return;
  }

  // Save IQ data in binary format
  std::ofstream outfile(args.rf.output_file, std::ios::binary);
  if (!outfile) {
    std::cerr << "Failed to open output file: " << args.rf.output_file
              << std::endl;
    return;
  }
  outfile.write(reinterpret_cast<const char *>(buffer.data()),
                received_samples * sizeof(std::complex<float>));
  outfile.close();

  std::cout << "IQ Data saved to " << args.rf.output_file << std::endl;
}

int main(int argc, char *argv[]) {
  all_args_t args;
  parse_args(args, argc, argv);
  collect_iq_data(args);
  return 0;
}
