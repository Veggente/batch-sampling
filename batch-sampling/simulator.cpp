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
    assert(t >= 0.0);
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

// Guaranteed arrival. A batch arrival happens with fixed batch size.
void Simulator::arrive_continuous_time(double time,
                                       std::mt19937 &rng) {  // NOLINT
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        cluster_[i].arrive_continuous_time(time, rng);
    }
}

// Guaranteed arrival. A batch arrival happens with Poisson batch size.
void Simulator::arrive_poisson(double time,
                               std::mt19937 &rng) {  // NOLINT
    // Determine batch size and check overflow.
    std::poisson_distribution<> pois(batch_size_);
    int64_t new_batch_size = pois(rng);
    while (new_batch_size*kMaxProbeRatio > num_servers_) {
        new_batch_size = pois(rng);
    }
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        cluster_[i].change_batch_size(new_batch_size);
        cluster_[i].arrive_continuous_time(time, rng);
    }
}

// Potential departure at a random server.
void Simulator::depart_single_continuous_time(double time,
        const std::string &filename_infix, std::mt19937 &rng,
        int log_indicator) {
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        cluster_[i].depart_single_continuous_time(time, filename_infix, rng,
                                                  log_indicator);
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

void Simulator::synopsize_continuous_time(const std::string &filename_infix) {
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        // Use scheduler-specific suffix combined with the infix.
        std::string filename_suffix = filename_infix+"_"+cluster_[i].suffix();
        cluster_[i].synopsize_continuous_time(filename_suffix);
    }
}

void Simulator::arrive_geometric(double time, std::mt19937 &rng) {  // NOLINT
    // Determine batch size.
    std::bernoulli_distribution bern(0.5);
    int64_t new_batch_size = 0;
    if (bern(rng)) {  // Geometric with small mean.
        std::geometric_distribution<> geom(1.0/kGeometricMeanSmall);
        new_batch_size = geom(rng);
    } else {  // Geometric with large mean.
        std::geometric_distribution<> geom(1.0/kGeometricMeanLarge);
        new_batch_size = geom(rng);
    }
    // Bounded at half server size.
    if (new_batch_size > num_servers_/2) {
        new_batch_size = num_servers_/2;
    }
    // Arrival.
    for (int i = starting_policy_index_; i < static_cast<int>(cluster_.size());
         ++i) {
        cluster_[i].change_batch_size(new_batch_size);
        if (new_batch_size > 0) {
            cluster_[i].arrive_continuous_time(time, rng);
        }
    }
}
