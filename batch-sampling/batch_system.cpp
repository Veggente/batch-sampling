//
//  batch_system.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "batch_system.h"

BatchSystem::BatchSystem() {
    controller_ = Controller();
    simulator_ = Simulator();
}

void BatchSystem::init(int64_t n, double a, double arr_pr, double total_time,
                       double r) {
    controller_.init(n, a, arr_pr, total_time, r);
    simulator_.init(controller_.num_server(),
                    controller_.arrival_rate_per_server(),
                    controller_.batch_size(), controller_.time_slot_length(),
                    controller_.probe_ratio());
}
