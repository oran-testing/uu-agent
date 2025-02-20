#ifndef IQ_COLLECTION_H
#define IQ_COLLECTION_H

#include "args.h"

#include <complex>
#include <fstream>
#include <uhd/stream.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <vector>
void collect_iq_data(const all_args_t &args);

#endif // !IQ_COLLECTION_H
