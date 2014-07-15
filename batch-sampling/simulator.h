//
//  simulator.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_SIMULATOR_H_
#define BATCH_SAMPLING_SIMULATOR_H_

#include <vector>
#include "./common.h"
#include "./cluster.h"

class Simulator {
public:
    Simulator();
    int init(int64_t n);
//    std::vector<Cluster> cluster() const {return cluster_;}
private:
    int64_t num_server_;
    double arrival_rate_per_server_;
    int64_t batch_size_;
    double time_slot_length_;
    std::vector<Cluster> cluster_;
};

#endif  // BATCH_SAMPLING_SIMULATOR_H_
