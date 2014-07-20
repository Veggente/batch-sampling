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
    std::string arrival_rate_str(argv[2]);
    std::string arrival_prob_str(argv[3]);
    std::string total_time_str(argv[4]);
    std::string probe_ratio_str(argv[5]);
    BatchSystem bs;
    int64_t num_servers = std::stoll(num_servers_str);
    double arrival_rate = std::stod(arrival_rate_str);
    double arrival_prob = std::stod(arrival_prob_str);
    double total_time = std::stod(total_time_str);
    double probe_ratio = std::stod(probe_ratio_str);
    bs.init(num_servers, arrival_rate, arrival_prob, total_time, probe_ratio);
    bs.show_config();
    std::mt19937 rng(0);
    bs.run(rng);
    return 0;
}

