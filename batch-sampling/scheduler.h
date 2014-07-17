//
//  scheduler.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_SCHEDULER_H_
#define BATCH_SAMPLING_SCHEDULER_H_

#include <random>
#include "./common.h"

Queues bswf(const Queues &queue, int64_t num_to_fill, std::mt19937 &rng);

class Scheduler {
public:
    Scheduler();
    Scheduler(Policy p, double r);
    double probe_ratio() const {return probe_ratio_;}
private:
    Policy policy_;
    double probe_ratio_;
};

#endif  // BATCH_SAMPLING_SCHEDULER_H_
