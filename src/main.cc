#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/types/tune_request.hpp>
#include <iostream>
#include <vector>

int main (int argc, char *argv[]) {
  uhd::device_addr_t dev_addr;
  uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(dev_addr);

  usrp->set_rx_rate(1e6);           // 1 MS/s
  usrp->set_rx_freq(uhd::tune_request_t(915e6)); // 915 MHz
  usrp->set_rx_gain(30);            // 30 dB gain

  std::cout << "USRP initialized successfully!" << std::endl;

  uhd::stream_args_t stream_args("fc32", "sc16"); // Complex floats in, complex shorts out
  uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

  size_t num_samps = rx_stream->get_max_num_samps();
  std::vector<std::complex<float>> buffer(num_samps);

  uhd::rx_metadata_t metadata;
  double duration_seconds = 10.0; // Receive for 10 seconds
  uhd::time_spec_t start_time = uhd::time_spec_t(0.1); // Start 0.1 seconds from now
  usrp->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);

  while (true) {
      size_t num_rx_samps = rx_stream->recv(&buffer.front(), buffer.size(), metadata);
      if (metadata.error_code != uhd::rx_metadata_t::ERROR_CODE_NONE) {
          std::cerr << "Error receiving samples: " << metadata.strerror() << std::endl;
          break;
      }

      std::cout << "First received sample: " << buffer[0] << std::endl;
  }

  usrp->issue_stream_cmd(uhd::stream_cmd_t::STREAM_MODE_STOP_CONTINUOUS);

  return 0;
}
