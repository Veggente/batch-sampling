#!/usr/bin/python
import os
import math

def exec_time(num_servers, batch_size, arrival_rate, arrival_prob):
    return 3000000*arrival_prob*batch_size/arrival_rate/num_servers

def main():
    num_servers = [10000]
    batch_size = 100;
    arrival_rate = [0.3]
    arrival_prob = 0.1
    probe_ratio = 2
    mit_indicator = 1
    for n in num_servers:
        for a in arrival_rate:
            total_time = exec_time(n, batch_size, a, arrival_prob)
            os.system("nice time ./batch-sampling "+str(n)+" "+str(batch_size)
                      +" "+str(a)+" "+str(arrival_prob)+" "+str(total_time)+" "
                      +str(probe_ratio)+" "+str(mit_indicator))

if __name__ == "__main__":
    main()
