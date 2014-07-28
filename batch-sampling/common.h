//
//  common.h
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#ifndef BATCH_SAMPLING_COMMON_H_
#define BATCH_SAMPLING_COMMON_H_

#include <vector>
#include <deque>

enum Policy {
    MIT,   // Mitzenmacher's power-of-d-choices.
    BS,    // Batch sampling.
    BSWF,  // Batch sampling with water filling.
    POLICY_COUNT
};

typedef std::vector<int64_t> Queues;
typedef std::deque<int64_t> BatchNumbers;
typedef std::vector<BatchNumbers> BatchQueues;

const int kNumInputArguments = 7;
const int kAHundred = 100;
const int kNumSynopses = 10;
// TODO(Veggente): Include input argument.
//const double kMeanBatchSize = 100.0;
const double kMaxProbeRatio = 2.0;

#endif  // BATCH_SAMPLING_COMMON_H_
