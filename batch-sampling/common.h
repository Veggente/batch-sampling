//
//  common.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_COMMON_H_
#define BATCH_SAMPLING_COMMON_H_

#include <vector>

enum Policy {
    MIT,  // Mitzenmacher's power-of-d-choices
    BS,  // batch sampling
    BSWF,  // batch sampling with water filling
    POLICY_COUNT
};

typedef std::vector<int64_t> Queues;

#endif  // BATCH_SAMPLING_COMMON_H_
