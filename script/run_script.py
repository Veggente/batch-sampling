#!/usr/bin/python
import os
import math

def main():
    num_servers = [100]
    batch_size = 20;
    arrival_rate = [0.5]
    total_time = 100
    probe_ratio = 1.6
    mit_indicator = 0
    log_indicator = 0
    for n in num_servers:
        for a in arrival_rate:
            os.system("nice time ./batch-sampling "+str(n)+" "+str(batch_size)
                      +" "+str(a)+" "+str(log_indicator)+" "+str(total_time)+" "
                      +str(probe_ratio)+" "+str(mit_indicator))

if __name__ == "__main__":
    main()
