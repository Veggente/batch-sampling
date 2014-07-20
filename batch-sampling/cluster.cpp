//
//  cluster.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./cluster.h"
#include <random>
#include <cassert>
#include <set>
#include <cmath>
#include <iostream>  // NOLINT
#include <algorithm>
#include <fstream>  // NOLINT
#include <string>
#include "./enum_parser.h"

Cluster::Cluster() {
    scheduler_ = Scheduler();
    time_slot_length_ = 1.0;
    num_servers_ = 0;
    batch_size_ = 0;
}

int Cluster::init(int64_t n, int64_t b, Policy p, double r, double t) {
    assert(n >= 0);
    num_servers_ = n;
    queue_length_ = Queues(n, 0);
    assert(r >= 1.0);
    scheduler_ = Scheduler(p, r);
    assert(t > 0.0);
    time_slot_length_ = t;
    assert(b >= 0);
    batch_size_ = b;
    return 0;
}

void Cluster::arrive(std::mt19937 &rng) {  // NOLINT
    int64_t num_probed_servers = std::llrint(batch_size_*
                                             scheduler_.probe_ratio());
    assert(num_probed_servers >= 0);
    assert(num_probed_servers <= num_servers_);
    // rand_sample returns ascending vector, so further randomization is done.
    Queues probed_servers = rand_sample(num_servers_, num_probed_servers, rng);
    std::shuffle(probed_servers.begin(), probed_servers.end(), rng);
    Queues probed_queues;
    for (int i = 0; i < static_cast<int>(probed_servers.size()); ++i) {
        // Base of probed_servers is 1.
        probed_queues.push_back(queue_length_[probed_servers[i]-1]);
    }
    Queues filled_queues;
    if (scheduler_.policy() == MIT) {
        filled_queues = mit(probed_queues, batch_size_,
                            scheduler_.probe_ratio(), rng);
    } else if (scheduler_.policy() == BS) {
        filled_queues = bs(probed_queues, batch_size_, rng);
    } else if (scheduler_.policy() == BSWF) {
        filled_queues = bswf(probed_queues, batch_size_, rng);
    }
    for (int i = 0; i < static_cast<int>(probed_servers.size()); ++i) {
        for (int64_t j = queue_length_[probed_servers[i]-1]+1;
             j <= filled_queues[i]; ++j) {
            auto it = num_servers_queue_at_least_.find(j);
            if (it == num_servers_queue_at_least_.end()) {
                num_servers_queue_at_least_[j] = 1;
            } else {
                ++it->second;
            }
        }
        queue_length_[probed_servers[i]-1] = filled_queues[i];
    }
}

void Cluster::depart(std::mt19937 &rng) {  // NOLINT
    // time_slot_length_ is the parameter of the Bernoulli service since the
    // service rate is 1.
    std::bernoulli_distribution bern(time_slot_length_);
    for (int64_t i = 0; i < num_servers_; ++i) {
        int64_t this_length = queue_length_[i];
        if (this_length > 0 && bern(rng)) {
            assert(num_servers_queue_at_least_.find(this_length) !=
                   num_servers_queue_at_least_.end());
            if (num_servers_queue_at_least_[this_length] == 1) {
                num_servers_queue_at_least_.erase(this_length);
            } else {
                --num_servers_queue_at_least_[this_length];
            }
            --queue_length_[i];
        }
    }
}

std::string Cluster::suffix() {
    EnumParser<Policy> parser_policy;
    std::string s = parser_policy.enum_to_string(scheduler_.policy())+"_"
                    +std::to_string(scheduler_.probe_ratio());
    return s;
}

void Cluster::log_queues(const std::string &filename) {
    std::ofstream out(filename, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename << "!"
            << std::endl;
        exit(1);
    }
    for (auto i : num_servers_queue_at_least_) {
        out << i.second << " ";
    }
    out << std::endl;
    out.close();
}

Queues rand_sample(int64_t n, int64_t m, std::mt19937 &rng) {  // NOLINT
    assert(n >= m);
    std::uniform_int_distribution<int64_t> dis(1, n);
    std::set<int64_t> chosen_set;
    while (static_cast<int>(chosen_set.size()) < m) {
        chosen_set.insert(dis(rng));
    }
    assert(static_cast<int>(chosen_set.size()) == m);
    Queues sampled_vector(chosen_set.begin(), chosen_set.end());
    return sampled_vector;
}
