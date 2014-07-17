//
//  scheduler.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./scheduler.h"
#include <cassert>
#include <algorithm>
#include "./cluster.h"

Scheduler::Scheduler() {
    policy_ = MIT;
    probe_ratio_ = 1.0;
}

Scheduler::Scheduler(Policy p, double r) {
    assert(r >= 1.0);
    policy_ = p;
    probe_ratio_ = r;
}

Queues bswf(const Queues &queue, int64_t num_to_fill, std::mt19937 &rng) {
    assert(!queue.empty());
    Queues sorted_queue = queue;
    std::sort(sorted_queue.begin(), sorted_queue.end());
    // sort by queue length
    int64_t water_level = sorted_queue[0];
    int64_t num_gainer = 0;
    int64_t num_big_gainer = 0;
    while (num_to_fill > 0) {
        ++water_level;
        while (sorted_queue[num_gainer] < water_level &&
               num_gainer < queue.size()) {
            ++num_gainer;
        }
        if (num_to_fill <= num_gainer) {
            num_big_gainer = num_to_fill;
            num_to_fill = 0;
        } else {
            num_to_fill = num_to_fill-num_gainer;
        }
    }
    // find water level
    Queues big_gainer = rand_sample(num_gainer, num_big_gainer, rng);
    int64_t gainer_index = 0;
    int64_t current_big_gainer = 0;
    Queues queue_after = queue;
    for (int64_t i = 0; i < queue.size(); ++i) {
        if (queue[i] < water_level) {
            ++gainer_index;
            if (gainer_index == big_gainer[current_big_gainer]) {
                queue_after[i] = water_level;
                ++current_big_gainer;
            } else {
                queue_after[i] = water_level-1;
            }
        }
    }
    // choose tie-break winners randomly
    return queue_after;
    // return
}
