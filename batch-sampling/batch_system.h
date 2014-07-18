//
//  batch_system.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_BATCH_SYSTEM_H_
#define BATCH_SAMPLING_BATCH_SYSTEM_H_

#include <cstdint>
#include "./controller.h"
#include "./simulator.h"

class BatchSystem {
public:  // NOLINT
    BatchSystem();
    void init(int64_t n, double a, double arr_pr, double total_time, double r);
private:  // NOLINT
    Controller controller_;
    Simulator simulator_;
};

#endif  // BATCH_SAMPLING_BATCH_SYSTEM_H_
