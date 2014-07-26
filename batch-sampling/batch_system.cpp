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

void BatchSystem::init(int64_t n, double a, double arr_pr, double total_time,
                       double r, int mit_indicator) {
    controller_.init(n, a, arr_pr, total_time, r);
    simulator_.init(controller_.num_servers(),
                    controller_.arrival_rate_per_server(),
                    controller_.batch_size(), controller_.time_slot_length(),
                    controller_.probe_ratio(), mit_indicator);
}

void BatchSystem::init(int64_t n, int64_t b, double a, double arr_pr,
                       double total_time, double r, int mit_indicator) {
    controller_.init(n, b, a, arr_pr, total_time, r);
    simulator_.init(controller_.num_servers(),
                    controller_.arrival_rate_per_server(),
                    controller_.batch_size(), controller_.time_slot_length(),
                    controller_.probe_ratio(), mit_indicator);
}

void BatchSystem::run(std::mt19937 &rng) {  // NOLINT
    int num_time_slots = controller_.num_time_slots();
    for (int i = 0; i < num_time_slots; ++i) {
        simulator_.arrive(i, rng);
        simulator_.depart(i, controller_.infix(), rng);
        // TODO(Veggente): possible speedup by generating filename once for all.
        simulator_.log_queues("queues_"+controller_.infix());
        // Output kNumSynopses-1 synopses in the middle of the simulation. The
        // synopsis data can be used to quickly compute desired statistics
        // without reading all the raw files.
        if (i > 0 &&
            i/(num_time_slots/kNumSynopses) !=
            (i-1)/(num_time_slots/kNumSynopses) &&
            i/(num_time_slots/kNumSynopses) < kNumSynopses) {
            simulator_.synopsize(controller_.infix());
        }
        controller_.progress_bar(i);
    }
    // Output a final synopsis.
    simulator_.synopsize(controller_.infix());
}

// TODO(Veggente): should migrate to Controller.
void BatchSystem::show_config() {
    std::cout << "===========Config===========" << std::endl;
    std::cout << "Number of servers: " << controller_.num_servers() << std::endl
    << "Batch size: " << controller_.batch_size() << std::endl
    << "Arrival rate: " << controller_.arrival_rate_per_server() << std::endl
    << "Arrival probability: " << controller_.arrival_probability() << std::endl
    << "Time slot length: " << controller_.time_slot_length() << std::endl
    << "Total time: " << controller_.total_time() << std::endl
    << "Probe ratio: " << controller_.probe_ratio() << std::endl
    << "Number of time slots: " << controller_.num_time_slots() << std::endl;
    std::cout << "============================" << std::endl;
}

void BatchSystem::run_continuous_time(std::mt19937 &rng) {  // NOLINT
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
            // TODO(Veggente): Pre-arrival queue length log.
            simulator_.log_queues_no_clock_tick("pre_arrival_queues_"
                                                +controller_.infix());
            // Arrival to clusters and update members.
            simulator_.arrive_continuous_time(time_, rng);
        } else {  // If departure happens, randomly choose a server to depart.
                  // If queue is not empty, task delay is recorded. If task is
                  // the last of batch, batch delay is recorded.
            // Time increments.
            time_ += inter_departure_time;
            // Randomly choose a queue to depart. Update task delay and batch
            // delay.
            simulator_.depart_single_continuous_time(time_, controller_.infix(),
                                                     rng);
            // TODO(Veggente): Post-departure queue length log.
            simulator_.log_queues_no_clock_tick("post_departure_queues_"
                                                +controller_.infix());
        }
        // TODO(Veggente): Post-event queue length log.
        simulator_.log_queues_no_clock_tick("queues_"+controller_.infix());
        // Clock tick.
        simulator_.clock_tick();
        // TODO(Veggente): Output synopsis if necessary.
        // TODO(Veggente): Continuous-time progress bar.
    }
    // TODO(Veggente): Final synopsis.
}
