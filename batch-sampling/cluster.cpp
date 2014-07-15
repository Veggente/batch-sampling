//
//  cluster.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./cluster.h"
#include <cassert>

Cluster::Cluster() {
    scheduler_ = Scheduler();
    time_slot_length_ = 1.0;
}

int Cluster::init(int64_t n, Policy p, double r, double t) {
    assert(n >= 0);
    queue_length_ = Queues(n, 0);
    assert(r >= 1.0);
    scheduler_ = Scheduler(p, r);
    assert(t > 0.0);
    time_slot_length_ = t;
    return 0;
}

void Cluster::arrive(std::mt19937 &rng) {
    // randomly pick d*m servers out of n
    // assign m customers onto the probed servers according to policy
}

void Cluster::depart(std::mt19937 &rng) {
    // generate Bernoulli random variables
    // depart
}

std::vector<int64_t> rand_sample(int64_t n, int64_t m) {
    std::vector<int64_t> sampled_vector(m, 0);
    
    return sampled_vector;
}
