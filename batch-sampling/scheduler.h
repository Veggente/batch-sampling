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

//TODO: use iterator arguments to allow sampling partial queues to speed up mit.
// Batch sampling with water filling algorithm.
Queues bswf(const Queues &queue, int64_t num_to_fill, std::mt19937 &rng);
// Batch sampling without water filling algorithm.
Queues bs(const Queues &queue, int64_t num_to_fill, std::mt19937 &rng);
// Mitzenmacher's power-of-d-choices algorithm
Queues mit(const Queues &queue, int64_t num_to_fill, double probe_ratio,
           std::mt19937 &rng);

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
