//
//  cluster.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_CLUSTER_H_
#define BATCH_SAMPLING_CLUSTER_H_

#include <random>
#include "./scheduler.h"
#include "./common.h"

class Cluster {
public:
    Cluster();
    int init(int64_t n, Policy p, double r, double t);
    int64_t num_server() const {return queue_length_.size();}
    Queues queue_length() const {return queue_length_;}
    void arrive(std::mt19937 &rng);
    void depart(std::mt19937 &rng);
private:
    Queues queue_length_;
    Scheduler scheduler_;
    double time_slot_length_;
};

#endif  // BATCH_SAMPLING_CLUSTER_H_
