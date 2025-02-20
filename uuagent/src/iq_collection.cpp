#include "iq_collection.h"

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
