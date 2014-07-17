//
//  controller.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_CONTROLLER_H_
#define BATCH_SAMPLING_CONTROLLER_H_

#include <cstdint>

class Controller {
public:
    Controller();
    void init(int64_t n, double a, double arr_pr, double total_time, double r);
    int64_t num_servers() const {return num_servers_;}
    int64_t batch_size() const {return batch_size_;}
    double arrival_rate_per_server() const {return arrival_rate_per_server_;}
    double time_slot_length() const {return time_slot_length_;}
    double probe_ratio() const {return probe_ratio_;}
private:
    int64_t num_servers_;
    int64_t batch_size_;
    double arrival_rate_per_server_;
    double arrival_probability_;
    double time_slot_length_;
    double total_time_;
    double probe_ratio_;
};

#endif  // BATCH_SAMPLING_CONTROLLER_H_
