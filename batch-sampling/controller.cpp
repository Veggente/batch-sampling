//
//  controller.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./controller.h"
#include <cmath>

Controller::Controller() {
    num_server_ = 0;
    batch_size_ = 0;
    arrival_rate_per_server_ = 1.0;
    arrival_probability_ = 1.0;
    time_slot_length_ = 1.0;
    total_time_ = 1.0;
    probe_ratio_ = 1.0;
}

void Controller::init(int64_t n, double a, double arr_pr, double total_time,
                      double r) {
    num_server_ = n;
    batch_size_ = static_cast<int64_t>(std::log2(num_server_))+1;
    arrival_rate_per_server_ = a;
    arrival_probability_ = arr_pr;
    time_slot_length_ = arrival_probability_*batch_size_/num_server_
        /arrival_rate_per_server_;
}
