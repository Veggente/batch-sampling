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
    void init(int64_t n, int64_t b, double a, double arr_pr, double total_time,
              double r);
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
    // Discrete-time version progress bar.
    void progress_bar(int time_slot);
    double batch_arrival_rate() const {return arrival_rate_per_server_
        *num_servers_/mean_batch_size_;}
    // Continuous-time version progress bar.
    void progress_bar(double time);
    bool is_synopsis_time(double time);
    void show_config(int log_indicator);
    bool variable_batch_indicator() const {return variable_batch_indicator_;}
private:  // NOLINT
    int64_t num_servers_;
    int64_t batch_size_;
    double arrival_rate_per_server_;
    double arrival_probability_;
    double time_slot_length_;
    double total_time_;
    double probe_ratio_;
    int progress_percentage_;
    int synopsis_counter_;
    double mean_batch_size_;
    bool variable_batch_indicator_;
};

#endif  // BATCH_SAMPLING_CONTROLLER_H_
