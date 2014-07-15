//
//  simulator.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./simulator.h"
#include <cassert>

Simulator::Simulator() {
    num_server_ = 0;
    arrival_rate_per_server_ = 0;
    batch_size_ = 1;
    time_slot_length_ = 1.0;
}

int Simulator::init(int64_t n) {
    assert(n >= 0);
    num_server_ = n;
    return 0;
}
