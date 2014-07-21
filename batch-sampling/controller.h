//
//  controller.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_CONTROLLER_H_
#define BATCH_SAMPLING_CONTROLLER_H_

#include <cstdint>
#include <string>

class Controller {
public:  // NOLINT
    Controller();
    void init(int64_t n, double a, double arr_pr, double total_time, double r);
    int64_t num_servers() const {return num_servers_;}
    int64_t batch_size() const {return batch_size_;}
    double arrival_rate_per_server() const {return arrival_rate_per_server_;}
    double time_slot_length() const {return time_slot_length_;}
    double probe_ratio() const {return probe_ratio_;}
    double total_time() const {return total_time_;}
    int num_time_slots() const {return static_cast<int>(total_time_
                                                        /time_slot_length_);}
    double arrival_probability() const {return arrival_probability_;}
    // TODO(Veggente): fewer precision for doubles.
    std::string infix() const;
    void progress_bar(int time_slot);
private:  // NOLINT
    int64_t num_servers_;
    int64_t batch_size_;
    double arrival_rate_per_server_;
    double arrival_probability_;
    double time_slot_length_;
    double total_time_;
    double probe_ratio_;
};

#endif  // BATCH_SAMPLING_CONTROLLER_H_
