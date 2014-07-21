//
//  batch_system.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./batch_system.h"
#include <iostream>  // NOLINT

BatchSystem::BatchSystem() {
    controller_ = Controller();
    simulator_ = Simulator();
}

void BatchSystem::init(int64_t n, double a, double arr_pr, double total_time,
                       double r) {
    controller_.init(n, a, arr_pr, total_time, r);
    simulator_.init(controller_.num_servers(),
                    controller_.arrival_rate_per_server(),
                    controller_.batch_size(), controller_.time_slot_length(),
                    controller_.probe_ratio());
}

void BatchSystem::run(std::mt19937 &rng) {  // NOLINT
    for (int i = 0; i < controller_.num_time_slots(); ++i) {
        simulator_.arrive(i, rng);
        simulator_.depart(i, controller_.infix(), rng);
        // TODO(Veggente): possible speedup by generating filename once for all.
        simulator_.log_queues("queues_"+controller_.infix());
        controller_.progress_bar(i);
    }
}

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
