#!/usr/bin/python
import os

def main():
    num_servers = [100]
    arrival_rate = [0.9]
    arrival_prob = 0.1
    total_time = 10
    probe_ratio = [2]
    mit_indicator = 1
    for n in num_servers:
        for a in arrival_rate:
            for r in probe_ratio:
                os.system("nice time ./batch-sampling "+str(n)+" "+str(a)+" "+str(arrival_prob)+" "+str(total_time)+" "+str(r)+" "+str(mit_indicator))

if __name__ == "__main__":
    main()
