#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <iostream>
#include <vector>

#include "args.h"

void parse_args(all_args_t *args) {

  bpo::options_description general("General options");

  general.add_options()("help,h", "Produce help message")(
      "version,v", "Print version information and exit");

  bpo::options_description common("Configuration options");

  common.add_options()(
      "rf.type", bpo::value<std::string>(&args->rf.rf_type)->default_value(0.0),
      "Force Tx and Rx sampling rate in Hz")(
      "rf.srate", bpo::value<double>(&args->rf.srate_hz)->default_value(0.0),
      "Force Tx and Rx sampling rate in Hz")(
      "rf.rx_gain", bpo::value<float>(&args->rf.rx_gain)->default_value(-1),
      "Front-end receiver gain")(
      "rf.tx_gain", bpo::value<float>(&args->rf.tx_gain)->default_value(-1),
      "Front-end transmitter gain (all channels)")(
      "rf.rx_freq", bpo::value<float>(&args->rf.rx_freq)->default_value(0),
      "RX frequency")("rf.tx_freq",
                      bpo::value<float>(&args->rf.tx_freq)->default_value(0),
                      "TX frequency");
}

int main(int argc, char *argv[]) {
  all_args_t uuagent_args;
  parse_args(&uuagent_args);
  return 0;
}
