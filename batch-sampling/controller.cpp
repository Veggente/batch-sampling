//
//  controller.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./controller.h"
#include <cmath>
#include <string>

Controller::Controller() {
    num_servers_ = 0;
    batch_size_ = 0;
    arrival_rate_per_server_ = 1.0;
    arrival_probability_ = 1.0;
    time_slot_length_ = 1.0;
    total_time_ = 1.0;
    probe_ratio_ = 1.0;
}

void Controller::init(int64_t n, double a, double arr_pr, double total_time,
                      double r) {
    num_servers_ = n;
    batch_size_ = std::llrint(std::log2(num_servers_));
    arrival_rate_per_server_ = a;
    arrival_probability_ = arr_pr;
    time_slot_length_ = arrival_probability_*batch_size_/num_servers_
        /arrival_rate_per_server_;
    total_time_ = total_time;
    probe_ratio_ = r;
}

std::string Controller::prefix() const {
    std::string s = "queues_n"+std::to_string(num_servers_)
                    +"_b"+std::to_string(batch_size_)
                    +"_a"+std::to_string(arrival_rate_per_server_)
                    +"_p"+std::to_string(arrival_probability_)
                    +"_t"+std::to_string(total_time_)
                    +"_r"+std::to_string(probe_ratio_);
    return s;
}
