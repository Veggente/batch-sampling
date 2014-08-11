//
//  controller.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./controller.h"
#include <cmath>
#include <string>
#include <iostream>  // NOLINT
#include "./common.h"

Controller::Controller() {
    num_servers_ = 0;
    batch_size_ = 0;
    arrival_rate_per_server_ = 1.0;
    arrival_probability_ = 1.0;
    time_slot_length_ = 1.0;
    total_time_ = 1.0;
    probe_ratio_ = 1.0;
    progress_percentage_ = 0;
    synopsis_counter_ = 1;
    mean_batch_size_ = 0.0;
    variable_batch_indicator_ = false;
}

void Controller::init(int64_t n, int64_t b, double a, double arr_pr,
                      double total_time, double r) {
    num_servers_ = n;
    batch_size_ = b;
    arrival_rate_per_server_ = a;
    arrival_probability_ = arr_pr;
    time_slot_length_ = arrival_probability_*batch_size_/num_servers_
        /arrival_rate_per_server_;
    total_time_ = total_time;
    probe_ratio_ = r;
    if (b > 0) {
        mean_batch_size_ = static_cast<double>(b);
    } else {
        mean_batch_size_ = static_cast<double>(kGeometricMeanBatchSize);
        variable_batch_indicator_ = true;
    }
}

std::string Controller::infix() const {
    std::string s = "n"+std::to_string(num_servers_)
                    +"_b"+std::to_string(batch_size_)
                    +"_a"+std::to_string(arrival_rate_per_server_)
                    +"_t"+std::to_string(total_time_)
                    +"_r"+std::to_string(probe_ratio_);
    return s;
}

void Controller::progress_bar(int time_slot) {
    if (time_slot == num_time_slots()-1) {
        // Spaces are for covering the previous display.
        std::cout << "\rDone!           " << std::endl;
    } else if (time_slot*kAHundred/num_time_slots() !=
               (time_slot-1)*kAHundred/num_time_slots()) {
        std::cout << "\r" << time_slot*kAHundred/num_time_slots()
                  << "% completed." << std::flush;
    }
}

void Controller::progress_bar(double time) {
    if (time/total_time_*kAHundred >
        static_cast<double>(progress_percentage_)) {
        if (progress_percentage_ == kAHundred) {
            std::cout << "\rDone!           " << std::endl;
        } else {
            std::cout << "\r" << progress_percentage_ << "% completed."
                      << std::flush;
            progress_percentage_ = std::rint(std::ceil(time/total_time_*
                                                       kAHundred));
        }
    }
}

bool Controller::is_synopsis_time(double time) {
    if (time/total_time_*kNumSynopses >
        static_cast<double>(synopsis_counter_)) {
        ++synopsis_counter_;
        return true;
    } else {
        return false;
    }
}

void Controller::show_config(int log_indicator) {
    std::cout << "==============Config==============" << std::endl;
    std::cout << "Number of servers: " << num_servers_ << std::endl;
    if (batch_size_ > 0) {
        std::cout << "Batch size: " << batch_size_ << std::endl;
    } else {
        std::cout << "Batch size: variable with mean " << mean_batch_size_
                  << std::endl;
    }
    std::cout << "Arrival rate: " << arrival_rate_per_server_ << std::endl
    << "Total time: " << total_time_ << std::endl
    << "Probe ratio: " << probe_ratio_ << std::endl
    << "Log all queues and delays: " << log_indicator << std::endl
    << "Expected number of events: "
    << (arrival_rate_per_server_/mean_batch_size_+1)*num_servers_*total_time_
    << std::endl;
    std::cout << "==================================" << std::endl;
}
