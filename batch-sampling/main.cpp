//
//  main.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include <random>
#include "./batch_system.h"

int main(int argc, const char * argv[]) {
    // TODO(Veggente): main arguments.
    BatchSystem bs;
    int64_t num_servers = 10;
    double arrival_rate = 0.9;
    double arrival_prob = 0.1;
    double total_time = 10.0;
    double probe_ratio = 2.0;
    bs.init(num_servers, arrival_rate, arrival_prob, total_time, probe_ratio);
    std::mt19937 rng(0);
    bs.run(rng);
    return 0;
}

