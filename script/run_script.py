#!/usr/bin/python
import os
import math

def exec_time(num_servers, batch_size, arrival_rate, arrival_prob):
    return 3000000*arrival_prob*batch_size/arrival_rate/num_servers

def main():
    num_servers = [200, 400, 800, 1600, 3200, 6400, 12800]
    arrival_rate = [0.3, 0.6, 0.9, 0.92, 0.94, 0.96, 0.98, 0.99]
    arrival_prob = 0.1
#    total_time = 4000
    probe_ratio = 2
    mit_indicator = 1
    for n in num_servers:
        for a in arrival_rate:
            if n == num_servers[0] or n == num_servers[3] or n == num_servers[6] or a == arrival_rate[0] or a == arrival_rate[2] or a == arrival_rate[7]:
                total_time = exec_time(n, math.log(n, 2), a, arrival_prob)
                os.system("nice time ./batch-sampling "+str(n)+" "+str(a)+" "+str(arrival_prob)+" "+str(total_time)+" "+str(probe_ratio)+" "+str(mit_indicator))

if __name__ == "__main__":
    main()
