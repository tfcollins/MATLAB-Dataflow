# MATLAB-Dataflow [![Build Status](https://travis-ci.org/travisfcollins/MATLAB-Dataflow.svg?branch=new_queuing)](https://travis-ci.org/travisfcollins/MATLAB-Dataflow)  

### Dataflow architecture for MATLAB  

This work provids thread level pipelining of MATLAB functions as well as general concurrency.  Resulting in significant speedup of current MATLAB based SDR implementations on multicore systems, or other HPC based applications.  Our lab has demonstrated by a 20x speed increase for a 802.11a receiver, running in real-time.

Required Packages/Software  
 - Python2
 - cmake (For testing only)
 - CppHeaderParser `sudo pip install CppHeaderParser`
 - MATLAB R2014a+ (Have not tested with previous versions)
