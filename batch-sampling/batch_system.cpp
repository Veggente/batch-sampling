//
//  batch_system.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./batch_system.h"
#include <random>
#include <iostream>  // NOLINT

BatchSystem::BatchSystem() {
    controller_ = Controller();
    simulator_ = Simulator();
}

void BatchSystem::init(int64_t n, int64_t b, double a, double arr_pr,
                       double total_time, double r, int mit_indicator) {
    controller_.init(n, b, a, arr_pr, total_time, r);
    simulator_.init(controller_.num_servers(),
                    controller_.arrival_rate_per_server(),
                    controller_.batch_size(), controller_.time_slot_length(),
                    controller_.probe_ratio(), mit_indicator);
}

void BatchSystem::show_config(int log_indicator) {
    controller_.show_config(log_indicator);
}

void BatchSystem::run_continuous_time(int log_indicator,
                                      std::mt19937 &rng) {  // NOLINT
    while (time_ < controller_.total_time()) {
        std::exponential_distribution<> arrival_dist(
            controller_.batch_arrival_rate());
        std::exponential_distribution<> departure_dist(
            controller_.num_servers());
        double inter_arrival_time = arrival_dist(rng);
        double inter_departure_time = departure_dist(rng);
        // If batch arrives, dispatch to clusters. Need to update queues, queue
        // lengths, quick queue lengths, remaining task counter, batch arrival
        // times.
        if (inter_arrival_time < inter_departure_time) {
            // Time increments.
            time_ += inter_arrival_time;
            // Arrival to clusters and update members.
            if (controller_.variable_batch_indicator()) {
                simulator_.arrive_geometric(time_, rng);
            } else {
                simulator_.arrive_continuous_time(time_, rng);
            }
        } else {  // If departure happens, randomly choose a server to depart.
                  // If queue is not empty, task delay is recorded. If task is
                  // the last of batch, batch delay is recorded.
            // Time increments.
            time_ += inter_departure_time;
            // Randomly choose a queue to depart. Update task delay and batch
            // delay.
            simulator_.depart_single_continuous_time(time_, controller_.infix(),
                                                     rng, log_indicator);
        }
        // Post-event queue length log.
        if (log_indicator != 0) {
            simulator_.log_queues_no_clock_tick("queues_"+controller_.infix());
        }
        // Clock tick.
        simulator_.clock_tick();
        // Output synopsis if necessary.
        if (controller_.is_synopsis_time(time_)) {
            simulator_.synopsize_continuous_time(controller_.infix());
        }
        // Continuous-time progress bar.
        controller_.progress_bar(time_);
    }
}
