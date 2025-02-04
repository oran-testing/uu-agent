

typedef struct all_args_s {
  rf_args_t rf;
} all_args_t;

typedef struct rf_args_s {
  std::string rf_type;
  std::string device_args;
  double srate_hz;
  float rx_gain;
  float tx_gain;
  float rx_freq;
  float tx_freq;
} rf_args_t;
