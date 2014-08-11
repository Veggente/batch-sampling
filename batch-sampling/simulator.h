//
//  simulator.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_SIMULATOR_H_
#define BATCH_SAMPLING_SIMULATOR_H_

#include <cstdint>
#include <random>
#include <vector>
#include <string>
#include "./cluster.h"

class Simulator {
public:  // NOLINT
    Simulator();
    // r is the probe ratio.
    void init(int64_t n, double a, int64_t b, double t, double r,
              int mit_indicator);
    void log_queues(const std::string &filename_prefix);
    void synopsize(const std::string &filename_infix);
    void arrive_continuous_time(double time, std::mt19937 &rng);  // NOLINT
    void depart_single_continuous_time(double time_,
                                       const std::string &filename_infix,
                                       std::mt19937 &rng, int log_indicator);
    void log_queues_no_clock_tick(const std::string&fileanme_prefix);
    void clock_tick();
    void synopsize_continuous_time(const std::string &filename_infix);
    void arrive_poisson(double time, std::mt19937 &rng);  // NOLINT
    void arrive_geometric(double time, std::mt19937 &rng);  // NOLINT
private:  // NOLINT
    int64_t num_servers_;
    double arrival_rate_per_server_;
    int64_t batch_size_;
    double time_slot_length_;
    std::vector<Cluster> cluster_;
    int starting_policy_index_;
};

#endif  // BATCH_SAMPLING_SIMULATOR_H_
