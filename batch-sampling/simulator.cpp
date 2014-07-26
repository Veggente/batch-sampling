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
#include <string>
#include "./common.h"

Simulator::Simulator() {
    num_servers_ = 0;
    arrival_rate_per_server_ = 1.0;
    batch_size_ = 1;
    time_slot_length_ = 1.0;
    starting_policy_index_ = 0;
}

void Simulator::init(int64_t n, double a, int64_t b, double t, double r,
                     int mit_indicator) {
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
    // If mit_indicator is 0, skip MIT.
    if (mit_indicator == 0) {
        starting_policy_index_ = 1;
    }
    for (int i = starting_policy_index_; i < POLICY_COUNT; ++i) {
        cluster_[i].init(n, b, static_cast<Policy>(i), r, t);
    }
}

// Potential arrival. A batch arrival happens only when the Bernoulli random
// variable turns out to be one.
void Simulator::arrive(int64_t time_slot, std::mt19937 &rng) {  // NOLINT
    double arrival_probability = arrival_rate_per_server_*num_servers_
                                 *time_slot_length_/batch_size_;
    std::bernoulli_distribution bern(arrival_probability);
    if (bern(rng)) {
        for (int i = starting_policy_index_;
             i < static_cast<int>(cluster_.size()); ++i) {
            cluster_[i].arrive(time_slot, rng);
        }
    }
}

void Simulator::depart(int64_t time_slot, const std::string &filename_prefix,
                       std::mt19937 &rng) {  // NOLINT
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        cluster_[i].depart(time_slot, filename_prefix, rng);
    }
}

void Simulator::log_queues(const std::string &filename_prefix) {
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        // Use scheduler-specific suffix combined with the prefix.
        std::string filename = filename_prefix+"_"+cluster_[i].suffix();
        cluster_[i].log_queues(filename);
        cluster_[i].clock_tick();
    }
}

void Simulator::synopsize(const std::string &filename_infix) {
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        // Use scheduler-specific suffix combined with the infix.
        std::string filename_suffix = filename_infix+"_"+cluster_[i].suffix();
        cluster_[i].synopsize(filename_suffix);
    }
}

// TODO(Veggente): Include accurate time.
// Guaranteed arrival. A batch arrival happens.
void Simulator::arrive_continuous_time(double time,
                                       std::mt19937 &rng) {  // NOLINT
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        cluster_[i].arrive_continuous_time(time, rng);
    }
}

// TODO(Veggente): Include accurate time.
// Potential departure at a random server.
void Simulator::depart_single_continuous_time(double time,
        const std::string &filename_infix, std::mt19937 &rng) {
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        cluster_[i].depart_single_continuous_time(time, filename_infix, rng);
    }
}

void Simulator::log_queues_no_clock_tick(const std::string &filename_prefix) {
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        // Use scheduler-specific suffix combined with the prefix.
        std::string filename = filename_prefix+"_"+cluster_[i].suffix();
        cluster_[i].log_queues(filename);
    }
}

void Simulator::clock_tick() {
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        cluster_[i].clock_tick();
    }
}
