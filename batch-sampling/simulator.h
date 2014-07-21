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
    void arrive(int64_t time_slot, std::mt19937 &rng);  // NOLINT
    void depart(int64_t time_slot, const std::string &filename_prefix,
                std::mt19937 &rng);  // NOLINT
    void log_queues(const std::string &filename_prefix);
private:  // NOLINT
    int64_t num_servers_;
    double arrival_rate_per_server_;
    int64_t batch_size_;
    double time_slot_length_;
    std::vector<Cluster> cluster_;
    int starting_policy_index_;
};

#endif  // BATCH_SAMPLING_SIMULATOR_H_
