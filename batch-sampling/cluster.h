//
//  cluster.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_CLUSTER_H_
#define BATCH_SAMPLING_CLUSTER_H_

#include <cstdint>
#include <random>
#include <vector>
#include <cassert>
#include <string>
#include "./scheduler.h"
#include "./common.h"

Queues rand_sample(int64_t n, int64_t m, std::mt19937 &rng);  // NOLINT

class Cluster {
public:  // NOLINT
    Cluster();
    int init(int64_t n, int64_t b, Policy p, double r, double t);
    void arrive(std::mt19937 &rng);  // NOLINT
    void depart(std::mt19937 &rng);  // NOLINT
    Queues queue_length() const {return queue_length_;}
    std::string suffix();
    void log_queues(const std::string &filename);
private:  // NOLINT
    Queues queue_length_;
    Scheduler scheduler_;
    double time_slot_length_;
    int64_t num_servers_;
    int64_t batch_size_;
};

#endif  // BATCH_SAMPLING_CLUSTER_H_
