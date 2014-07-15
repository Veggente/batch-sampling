//
//  scheduler.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_SCHEDULER_H_
#define BATCH_SAMPLING_SCHEDULER_H_

#include "./common.h"

class Scheduler {
public:
    Scheduler();
    Scheduler(Policy p, double r);
private:
    Policy policy_;
    double probe_ratio_;
};

#endif  // BATCH_SAMPLING_SCHEDULER_H_
