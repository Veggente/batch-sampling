//
//  main.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include <random>
#include <iostream>  // NOLINT
#include <string>
#include "./batch_system.h"
#include "./common.h"

int main(int argc, const char * argv[]) {
    if (argc != kNumInputArguments+1) {
        std::cerr << "Invalid number of arguments!" << std::endl;
        return 1;
    }
    std::string num_servers_str(argv[1]);
    std::string batch_size_str(argv[2]);
    std::string arrival_rate_str(argv[3]);
    // TODO(Veggente): remove arrival probability for continuous-time.
//    std::string arrival_prob_str(argv[4]);
    std::string log_all_queues_str(argv[4]);
    std::string total_time_str(argv[5]);
    std::string probe_ratio_str(argv[6]);
    std::string mit_indicator_str(argv[7]);
    BatchSystem bs;
    int64_t num_servers = std::stoll(num_servers_str);
    int64_t batch_size = std::stoll(batch_size_str);
    double arrival_rate = std::stod(arrival_rate_str);
//    double arrival_prob = std::stod(arrival_prob_str);
    int log_all_queues = std::stoi(log_all_queues_str);
    double total_time = std::stod(total_time_str);
    double probe_ratio = std::stod(probe_ratio_str);
    int mit_indicator = std::stoi(mit_indicator_str);
    bs.init(num_servers, batch_size, arrival_rate, 0.0, total_time,
            probe_ratio, mit_indicator);
    bs.show_config(log_all_queues);
    std::mt19937 rng(0);
//    bs.run(rng);
    bs.run_continuous_time(log_all_queues, rng);
    return 0;
}

