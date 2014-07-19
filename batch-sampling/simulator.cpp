//
//  simulator.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./simulator.h"
#include <random>
#include <cassert>
#include <vector>
#include "./common.h"

Simulator::Simulator() {
    num_servers_ = 0;
    arrival_rate_per_server_ = 1.0;
    batch_size_ = 1;
    time_slot_length_ = 1.0;
}

void Simulator::init(int64_t n, double a, int64_t b, double t, double r) {
    assert(n >= 0);
    num_servers_ = n;
    assert(a >= 0.0);
    arrival_rate_per_server_ = a;
    assert(b >= 0);
    batch_size_ = b;
    assert(t > 0.0);
    time_slot_length_ = t;
    assert(r >= 1.0);
    cluster_ = std::vector<Cluster>(POLICY_COUNT, Cluster());
    for (int i = 0; i < POLICY_COUNT; ++i) {
        cluster_[i].init(n, b, static_cast<Policy>(i), r, t);
    }
}

void Simulator::arrive(std::mt19937 &rng) {  // NOLINT
    double arrival_probability = arrival_rate_per_server_*num_servers_
                                 *time_slot_length_/batch_size_;
    std::bernoulli_distribution bern(arrival_probability);
    if (bern(rng)) {
        for (int i = 0; i < cluster_.size(); ++i) {
            cluster_[i].arrive(rng);
        }
    }
}

void Simulator::depart(std::mt19937 &rng) {  // NOLINT
    for (int i = 0; i < cluster_.size(); ++i) {
        cluster_[i].depart(rng);
    }
}

void Simulator::log_queues(const std::string &filename_prefix) {
    for (int i = 0; i < cluster_.size(); ++i) {
        // Use scheduler-specific suffix combined with the prefix.
        std::string filename = filename_prefix+"_"+cluster_[i].suffix();
        cluster_[i].log_queues(filename);
    }
}
