//
//  scheduler.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "scheduler.h"
#include <cassert>

Scheduler::Scheduler() {
    policy_ = MIT;
    probe_ratio_ = 1;
}

Scheduler::Scheduler(Policy p, double r) {
    assert(r >= 1.0);
    policy_ = p;
    probe_ratio_ = r;
}
