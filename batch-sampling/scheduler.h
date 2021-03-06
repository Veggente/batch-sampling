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

// Batch sampling with water filling algorithm.
Queues bswf(const Queues &queue, int64_t num_to_fill, std::mt19937 &rng);
// Batch sampling without water filling algorithm.
Queues bs(const Queues &queue, int64_t num_to_fill, std::mt19937 &rng);
// Mitzenmacher's power-of-d-choices algorithm
Queues mit(const Queues &queue, int64_t num_to_fill, double probe_ratio,
           std::mt19937 &rng);

class Scheduler {
public:  // NOLINT
    Scheduler();
    Scheduler(Policy p, double r);
    double probe_ratio() const {return probe_ratio_;}
    Policy policy() const {return policy_;}
private:  // NOLINT
    Policy policy_;
    double probe_ratio_;
};

#endif  // BATCH_SAMPLING_SCHEDULER_H_
