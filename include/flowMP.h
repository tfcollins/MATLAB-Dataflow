// Dataflow Backbone
// author: Travis Collins (travisfcollins@gmail.com)
//
// Last Update: 8/20/2015

#ifndef FLOWMP_H
#define FLOWMP_H

#include <iostream>
#include <fstream>
#include <queue>
#include <boost/thread.hpp>
#include <chrono>
#include <atomic>
#include <functional>
#include <boost/scoped_ptr.hpp>
#include <vector>
#include <time.h>
#include "readerwriterqueue.h"
#include "atomicops.h"
#ifdef UNIX
  #include <sys/prctl.h> //Needed for thread naming
#endif
#include <memory>

// Uncomment to always enable benchmarking otherwise rely on compiler flag
// #define BENCHMARKING true

#define TIMING_SAMPLES 1000000

// #include "utils.h"

// If queue grows large warn since memory may be fully consumed eventually
#define WARNINGQSIZE 300

// Default functional placeholders
//void blankz(){}
//std::function<void()> blankTemp = blankz;

// Type declarations
// typedef std::shared_ptr<void> sptr_void_s;
typedef void* sptr_void_s;
typedef std::vector<sptr_void_s> voidvec;
typedef std::vector<sptr_void_s> OUTPUTS;
typedef std::vector<sptr_void_s> INPUTS;


// Class to be threaded
class Worker
{
public:
        // Constructor
        Worker(std::function<voidvec(voidvec,int*)> ProcessData,
               int numInputs, int numOutputs,
               std::function<void()> FunctionInit,         //= blankTemp
               std::function<void()> FunctionCleanup,
               std::string BlockName);

        // Processing/Setup/Cleanup functions
        std::function<voidvec(voidvec,int*)> m_ProcessData;
        std::function<void()> m_FunctionInit;
        std::function<void()> m_FunctionCleanup;

        // User defined parameters
        int m_NumInputs;
        int m_NumOutputs;
        // Parameters
        std::atomic<bool> m_StopThread;
        boost::thread m_BlockThread;
        // MIMO Parameters
        std::vector<boost::shared_ptr<std::atomic<int> > > m_InputQueueSizes;
        std::vector<boost::shared_ptr<std::atomic<int> > > m_OutputQueueSizes;
        std::vector<boost::shared_ptr<boost::mutex> >                m_InputMutexs;
        std::vector<boost::shared_ptr<boost::mutex> >                m_OutputMutexs;
        std::vector<boost::shared_ptr<boost::condition_variable> >   m_InputConds;
        std::vector<boost::shared_ptr<boost::condition_variable> >   m_OutputConds;
        std::vector<boost::shared_ptr<moodycamel::BlockingReaderWriterQueue<sptr_void_s> > >           m_InputQueues;
        std::vector<boost::shared_ptr<moodycamel::BlockingReaderWriterQueue<sptr_void_s> > >           m_OutputQueues;
        std::string m_BlockName;

        // Extra Benchmarking Parameters
        bool m_BenchMarking;
        int m_BenchMarkingCount;
        std::atomic<bool> m_BenchMarkComplete;
        std::chrono::high_resolution_clock::time_point m_BM_START;
        std::chrono::high_resolution_clock::time_point m_BM_END;
        std::vector<std::chrono::high_resolution_clock::time_point> m_BenchMarkingStarts;
        std::vector<std::chrono::high_resolution_clock::time_point> m_BenchMarkingEnds;

        // Data management functions
        // INPUTS
        voidvec readFromInputQueues();
        sptr_void_s readFromInputQueue(int inport);
        // OUTPUTS
        void addToOutputQueues(voidvec ProcessedDataVector);
        void addToOutputQueue(sptr_void_s processedData, int outport);
        void notifyConnectedBlocks();
        // In and Out Process Block
        void block();
        void block_source();
        void block_sink();
        // Block Runners
        void run();
        void run_source();
        void run_sink();

        //Destructor
        // ~Worker()
        // {
        //         for(uint port=0; port<m_OutputConds.size(); port++)
        //         {
        //                 m_OutputConds[port].reset();
        //                 //(*(m_OutputMutexs[port])).unlock();
        //                 (m_OutputMutexs[port]).reset(); //SEG FAULTING HERE
        //         }
        // }

};


#endif
