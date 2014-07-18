//
//  cluster.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./cluster.h"
#include <cassert>
#include <random>
#include <set>
#include <cmath>

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

void Cluster::arrive(std::mt19937 &rng) {
    int64_t num_probed_servers = std::llrint(batch_size_*
                                             scheduler_.probe_ratio());
    assert(num_probed_servers >= 0);
    assert(num_probed_servers <= num_servers_);
    Queues probed_server = rand_sample(num_servers_, num_probed_servers, rng);
    //TODO: assign m customers onto the probed servers according to policy.
}

void Cluster::depart(std::mt19937 &rng) {
    //TODO: generate Bernoulli random variables.
    //TODO: depart.
}

Queues rand_sample(int64_t n, int64_t m, std::mt19937 &rng) {
    assert(n >= m);
    std::uniform_int_distribution<int64_t> dis(1, n);
    std::set<int64_t> chosen_set;
    while (chosen_set.size() < m) {
        chosen_set.insert(dis(rng));
    }
    assert(chosen_set.size() == m);
    Queues sampled_vector(chosen_set.begin(), chosen_set.end());
    return sampled_vector;
}
