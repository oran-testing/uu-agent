#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

#include <boost/program_options.hpp>
#include <cstddef>
#include <fstream>
#include <iostream>

#include "args.h"
#include "iq_collection.h"

namespace bpo = boost::program_options;

// Parse command-line arguments
void parse_args(all_args_t &args, int argc, char *argv[]) {
  std::string conf_filepath = "";
  bpo::options_description general("General options");
  general.add_options()("help,h", "Produce help message")(
      "version,v", "Print version information and exit")(
      "config,c", bpo::value<std::string>(&conf_filepath),
      "Configuration file");

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
      "Deivice arguments for RF");

  bpo::options_description all_options;
  all_options.add(general).add(common);

  bpo::variables_map vm;
  bpo::store(bpo::parse_command_line(argc, argv, all_options), vm);
  bpo::notify(vm);

  if (vm.count("help")) {
    std::cout << all_options << std::endl;
    exit(0);
  }

  if (vm.count("config")) {
    std::ifstream config_file(conf_filepath);
    if (config_file) {
      bpo::store(bpo::parse_config_file(config_file, common), vm);
      bpo::notify(vm);
    } else {
      std::cerr << "Failed to open config file: " << conf_filepath << std::endl;
      exit(1);
    }
  }
}

int main(int argc, char *argv[]) {
  all_args_t args;
  parse_args(args, argc, argv);
  collect_iq_data(args);
  return 0;
}
