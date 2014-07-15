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
    assert(t > 0);
    time_slot_length_ = t;
    return 0;
}
