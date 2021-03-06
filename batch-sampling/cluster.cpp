//
//  cluster.cpp
//  batch-sampling
//
//  Copyright (c) 2014 Xiaohan Kang. All rights reserved.
//

#include "./cluster.h"
#include <random>
#include <cassert>
#include <set>
#include <cmath>
#include <iostream>  // NOLINT
#include <algorithm>
#include <fstream>  // NOLINT
#include <string>
#include "./enum_parser.h"

Cluster::Cluster() {
    scheduler_ = Scheduler();
    time_slot_length_ = 1.0;
    num_servers_ = 0;
    batch_size_ = 0;
    cumulative_batch_delay_ = 0;
    num_batches_completed_ = 0;
    cumulative_task_delay_ = 0;
    num_tasks_completed_ = 0;
    time_slot_ = 0;
    cumulative_batch_delay_cont_ = 0.0;
    cumulative_task_delay_cont_ = 0.0;
}

void Cluster::init(int64_t n, int64_t b, Policy p, double r, double t) {
    assert(n >= 0);
    num_servers_ = n;
    queue_length_ = Queues(n, 0);
    assert(r >= 1.0);
    scheduler_ = Scheduler(p, r);
    assert(t >= 0.0);
    time_slot_length_ = t;
    assert(b >= 0);
    batch_size_ = b;
    batch_queue_ = BatchQueues(n, BatchNumbers());
}

void Cluster::arrive(int64_t time_slot, std::mt19937 &rng) {  // NOLINT
    int64_t num_probed_servers = std::llrint(batch_size_*
                                             scheduler_.probe_ratio());
    assert(num_probed_servers >= 0);
    assert(num_probed_servers <= num_servers_);
    // rand_sample returns ascending vector, so further randomization is done.
    Queues probed_servers = rand_sample(num_servers_, num_probed_servers, rng);
    std::shuffle(probed_servers.begin(), probed_servers.end(), rng);
    Queues probed_queues;
    for (int i = 0; i < static_cast<int>(probed_servers.size()); ++i) {
        // Base of probed_servers is 1.
        probed_queues.push_back(queue_length_[probed_servers[i]-1]);
    }
    Queues filled_queues;
    if (scheduler_.policy() == MIT) {
        filled_queues = mit(probed_queues, batch_size_,
                            scheduler_.probe_ratio(), rng);
    } else if (scheduler_.policy() == BS) {
        filled_queues = bs(probed_queues, batch_size_, rng);
    } else if (scheduler_.policy() == BSWF) {
        filled_queues = bswf(probed_queues, batch_size_, rng);
    } else {
        assert(false);
    }
    for (int i = 0; i < static_cast<int>(probed_servers.size()); ++i) {
        for (int64_t j = queue_length_[probed_servers[i]-1]+1;
             j <= filled_queues[i]; ++j) {
            auto it = num_servers_queue_at_least_.find(j);
            if (it == num_servers_queue_at_least_.end()) {
                num_servers_queue_at_least_[j] = 1;
            } else {
                ++it->second;
            }
        }
        // Copies of the batch number, i.e. time_slot, are appended at the end
        // of batch queues.
        batch_queue_[probed_servers[i]-1].insert(
             batch_queue_[probed_servers[i]-1].end(),
             filled_queues[i]-queue_length_[probed_servers[i]-1],
             time_slot);
        queue_length_[probed_servers[i]-1] = filled_queues[i];
    }
    assert(num_remaining_tasks_.find(time_slot) == num_remaining_tasks_.end());
    num_remaining_tasks_[time_slot] = static_cast<int>(batch_size_);
}

std::string Cluster::suffix() {
    EnumParser<Policy> parser_policy;
    std::string s = parser_policy.enum_to_string(scheduler_.policy())+"_"
                    +std::to_string(scheduler_.probe_ratio());
    return s;
}

void Cluster::log_queues(const std::string &filename) {
    std::ofstream out(filename, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename << "!"
            << std::endl;
        exit(1);
    }
    for (auto i : num_servers_queue_at_least_) {
        out << i.second << " ";
    }
    out << std::endl;
    out.close();
}

void Cluster::log_delay(const std::string &filename, int64_t arrival_time,
                              int64_t completion_time) {
    std::ofstream out(filename, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename << "!"
                  << std::endl;
        exit(1);
    }
    out << arrival_time << " " << completion_time << std::endl;
    out.close();
}

void Cluster::synopsize(const std::string &filename_suffix) {
    // Batch delay synopsis.
    std::string filename_batch = "batch_synopsis_"+filename_suffix;
    std::ofstream out(filename_batch, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename_batch << "!"
                  << std::endl;
        exit(1);
    }
    out << num_batches_completed_ << " " << cumulative_batch_delay_
        << std::endl;
    out.close();
    // Task delay synopsis.
    std::string filename_task = "task_synopsis_"+filename_suffix;
    out.open(filename_task, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename_task << "!"
                  << std::endl;
        exit(1);
    }
    out << num_tasks_completed_ << " " << cumulative_task_delay_ << std::endl;
    out.close();
    // Queue length synopsis.
    std::string filename_queue = "queue_synopsis_"+filename_suffix;
    out.open(filename_queue, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename_queue << "!"
                  << std::endl;
        exit(1);
    }
    out << time_slot_;
    for (auto const &queue : cumulative_num_servers_queue_at_least_) {
        out << " " << queue.second;
    }
    out << std::endl;
}
void Cluster::clock_tick() {
    ++time_slot_;
    // Maintain cumulative queues.
    for (auto const &queue : num_servers_queue_at_least_) {
        cumulative_num_servers_queue_at_least_[queue.first] += queue.second;
    }
}

void Cluster::arrive_continuous_time(double time,
                                     std::mt19937 &rng) {  // NOLINT
    arrive(time_slot_, rng);
    // Record batch arrival time.
    assert(batch_arrival_time_.find(time_slot_) == batch_arrival_time_.end());
    batch_arrival_time_[time_slot_] = time;
}

void Cluster::depart_single_continuous_time(double time,
        const std::string &filename_infix, std::mt19937 &rng,
        int log_indicator) {
    // Randomly choose a server.
    std::uniform_int_distribution<int64_t> unif_dist(0, num_servers_-1);
    int64_t chosen_one = unif_dist(rng);
    // Depart if possible.
    assert(queue_length_[chosen_one] == batch_queue_[chosen_one].size());
    int64_t this_length = queue_length_[chosen_one];
    if (this_length > 0) {
        assert(num_servers_queue_at_least_.find(this_length) !=
               num_servers_queue_at_least_.end());
        if (num_servers_queue_at_least_[this_length] == 1) {
            num_servers_queue_at_least_.erase(this_length);
        } else {
            --num_servers_queue_at_least_[this_length];
        }
        --queue_length_[chosen_one];
        int64_t depart_batch_number = batch_queue_[chosen_one].front();
        assert(num_remaining_tasks_.find(depart_batch_number) !=
               num_remaining_tasks_.end());
        double delay = time-batch_arrival_time_[depart_batch_number];
        assert(delay > 0.0);
        if (num_remaining_tasks_[depart_batch_number] == 1) {
            // Erase maps.
            num_remaining_tasks_.erase(depart_batch_number);
            batch_arrival_time_.erase(depart_batch_number);
            // Log batch delay.
            if (log_indicator != 0) {
                std::string filename = "batch_delays_"+filename_infix+"_"
                                        +suffix();
                log_delay_continuous_time(filename, depart_batch_number,
                                          time_slot_, delay);
            }
            // Maintain batch completion counter and cumulative batch delay.
            ++num_batches_completed_;
            cumulative_batch_delay_cont_ += delay;
        } else {
            --num_remaining_tasks_[depart_batch_number];
        }
        batch_queue_[chosen_one].pop_front();
        // Log task delay.
        if (log_indicator != 0) {
            std::string filename = "task_delays_"+filename_infix+"_"+suffix();
            log_delay_continuous_time(filename, depart_batch_number, time_slot_,
                                      delay);
        }
        // Maintain task completion counter and cumulative task delay.
        ++num_tasks_completed_;
        cumulative_task_delay_cont_ += delay;
    }
}

void Cluster::log_delay_continuous_time(const std::string &filename,
                                        int64_t arrival_time_slot,
                                        int64_t completion_time_slot,
                                        double delay) {
    std::ofstream out(filename, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename << "!"
        << std::endl;
        exit(1);
    }
    out << arrival_time_slot << " " << completion_time_slot << " " << delay
        << std::endl;
    out.close();
}

void Cluster::synopsize_continuous_time(const std::string &filename_suffix) {
    // Batch delay synopsis.
    std::string filename_batch = "batch_synopsis_"+filename_suffix;
    std::ofstream out(filename_batch, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename_batch << "!"
        << std::endl;
        exit(1);
    }
    out << num_batches_completed_ << " " << cumulative_batch_delay_cont_
        << std::endl;
    out.close();
    // Task delay synopsis.
    std::string filename_task = "task_synopsis_"+filename_suffix;
    out.open(filename_task, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename_task << "!"
        << std::endl;
        exit(1);
    }
    out << num_tasks_completed_ << " " << cumulative_task_delay_cont_
        << std::endl;
    out.close();
    // Queue length synopsis.
    std::string filename_queue = "queue_synopsis_"+filename_suffix;
    out.open(filename_queue, std::ofstream::app);
    if (!out) {
        std::cerr << "Error: could not open file " << filename_queue << "!"
        << std::endl;
        exit(1);
    }
    out << time_slot_;
    for (auto const &queue : cumulative_num_servers_queue_at_least_) {
        out << " " << queue.second;
    }
    out << std::endl;
}

void Cluster::change_batch_size(int64_t new_batch_size) {
    assert(new_batch_size*scheduler_.probe_ratio() <= num_servers_);
    batch_size_ = new_batch_size;
}

Queues rand_sample(int64_t n, int64_t m, std::mt19937 &rng) {  // NOLINT
    assert(n >= m);
    std::uniform_int_distribution<int64_t> dis(1, n);
    std::set<int64_t> chosen_set;
    while (static_cast<int>(chosen_set.size()) < m) {
        chosen_set.insert(dis(rng));
    }
    assert(static_cast<int>(chosen_set.size()) == m);
    Queues sampled_vector(chosen_set.begin(), chosen_set.end());
    return sampled_vector;
}
