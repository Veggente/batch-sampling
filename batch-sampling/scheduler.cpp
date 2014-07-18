//
//  scheduler.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./scheduler.h"
#include <cassert>
#include <algorithm>
#include <cmath>
#include "/usr/local/include/prettyprint.hpp"
#include "./cluster.h"

Scheduler::Scheduler() {
    policy_ = MIT;
    probe_ratio_ = 1.0;
}

Scheduler::Scheduler(Policy p, double r) {
    assert(r >= 1.0);
    policy_ = p;
    probe_ratio_ = r;
}

Queues bswf(const Queues &queue, int64_t num_to_fill, std::mt19937 &rng) {
    assert(!queue.empty());
    assert(num_to_fill > 0);
    // Sort the queue by length.
    Queues sorted_queue = queue;
    std::sort(sorted_queue.begin(), sorted_queue.end());
    // water_level is the highest level the water in num_to_fill ever reaches.
    // We find the actual water level given the water amount num_to_fill by
    // gradually increasing the water level starting from the shortest queue.
    int64_t water_level = sorted_queue[0];
    // num_gainers is the number of queues that were originally strictly below
    // the water_level, i.e., those who may gain some water.
    int64_t num_gainers = 0;
    // num_big_gainers is the number of gainers that win the tie-breaking.
    int64_t num_big_gainers = 0;
    while (num_to_fill > 0) {
        // If the water in num_to_fill is not used up, we increase the water
        // level by one.
        ++water_level;
        // Find the number of gainers for the current water level.
        while (sorted_queue[num_gainers] < water_level &&
               num_gainers < queue.size()) {
            ++num_gainers;
        }
        // Current water level is the actual water level if water in num_to_fill
        // is used up.
        if (num_to_fill <= num_gainers) {
            num_big_gainers = num_to_fill;
            num_to_fill = 0;
        } else {
            num_to_fill = num_to_fill-num_gainers;
        }
    }
    // Randomly choose tie-breaking winners.
    Queues big_gainer = rand_sample(num_gainers, num_big_gainers, rng);
    assert(std::is_sorted(big_gainer.begin(), big_gainer.end()));
    int64_t gainer_index = 0;
    int64_t current_big_gainer_index = 0;
    Queues queue_after = queue;
    for (int64_t i = 0; i < queue.size(); ++i) {
        if (queue[i] < water_level) {
            ++gainer_index;
            if (current_big_gainer_index < big_gainer.size() &&
                gainer_index == big_gainer[current_big_gainer_index]) {
                queue_after[i] = water_level;
                ++current_big_gainer_index;
            } else {
                queue_after[i] = water_level-1;
            }
        }
    }
    return queue_after;
}

Queues bs(const Queues &queue, int64_t num_to_fill, std::mt19937 &rng) {
    assert(!queue.empty());
    assert(queue.size() >= num_to_fill);
    assert(num_to_fill > 0);
    // Sort the queue by length.
    Queues sorted_queue = queue;
    std::sort(sorted_queue.begin(), sorted_queue.end());
    // qualifying_line is the line below which placement of water is guaranteed.
    int64_t qualifying_line = sorted_queue[num_to_fill-1];
    // num_openings is the number of num_to_fill left at exactly
    // qualifying_line.
    int64_t num_openings = 0;  // initialization
    // first_applicant_index is the index of the first queue with length exactly
    // equal to qualifying_line.
    int64_t first_applicant_index = num_to_fill-1;
    while (first_applicant_index > 0 &&
           sorted_queue[first_applicant_index-1] ==
           sorted_queue[num_to_fill-1]) {
        --first_applicant_index;
    }
    num_openings = num_to_fill-first_applicant_index;
    // num_applicants is the number of queues with length exactly equal to the
    // qualifying line.
    int64_t num_applicants = 0;                    // initialization
    int64_t last_applicant_index = num_to_fill-1;  // initialization
    // The first condition must come first because if the first condition does
    // not hold, the second will overflow.
    while (last_applicant_index < queue.size()-1 &&
           sorted_queue[last_applicant_index+1] ==
           sorted_queue[num_to_fill-1]) {
        ++last_applicant_index;
    }
    num_applicants = last_applicant_index-first_applicant_index+1;
    // Randomly choose tie-breaking winners.
    Queues offers = rand_sample(num_applicants, num_openings, rng);
    assert(std::is_sorted(offers.begin(), offers.end()));
    int64_t applicant_index = 0;
    int64_t offer_index = 0;
    Queues queue_after = queue;
    for (int64_t i = 0; i < queue.size(); ++i) {
        if (queue[i] < qualifying_line) {
            ++queue_after[i];
        } else if (offer_index < num_openings && queue[i] == qualifying_line) {
            ++applicant_index;
            if (applicant_index == offers[offer_index]) {
                ++queue_after[i];
                ++offer_index;
            }
        }
    }
    return queue_after;
}

Queues mit(const Queues &queue, int64_t num_to_fill, double probe_ratio,
           std::mt19937 &rng) {
    assert(num_to_fill > 0);
    int probe_ratio_int = std::rint(probe_ratio);
    assert(queue.size() == num_to_fill*probe_ratio_int);
    Queues queue_after;
    Queues temp_queue;
    Queues temp_queue_after;
    for (int i = 0; i < num_to_fill; ++i) {
        // Copy part of queue to temp_queue.
        temp_queue.assign(queue.begin()+i*probe_ratio_int,
                          queue.begin()+(i+1)*probe_ratio_int);
        // Do BSWF with num_to_fill == 1.
        temp_queue_after = bswf(temp_queue, 1, rng);
        // Push the results to the back of queue_after.
        queue_after.insert(queue_after.end(), temp_queue_after.begin(),
                           temp_queue_after.end());
    }
    return queue_after;
}
