// Dataflow Backbone
// author: Travis Collins (travisfcollins@gmail.com)
//
// Last Update: 8/20/2015

#ifndef FLOWMP_H
#define FLOWMP_H

#include <iostream>
#include <queue>
#include <boost/thread.hpp>
#include <atomic>
#include <functional>
#include <boost/scoped_ptr.hpp>
#include <vector>
#include <time.h>
#ifdef UNIX
  #include <sys/prctl.h> //Needed for thread naming
#endif

// #include "utils.h"

// If queue grows large warn since memory may be fully consumed eventually
#define WARNINGQSIZE 300

// Default functional placeholders
//void blankz(){}
//std::function<void()> blankTemp = blankz;

// Type declarations
typedef std::vector<void*> voidvec;
typedef std::vector<void*> OUTPUTS;
typedef std::vector<void*> INPUTS;

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
        std::atomic<bool>                              m_StopThread;
        boost::thread m_BlockThread;
        // MIMO Parameters
        std::vector<boost::shared_ptr<std::atomic<int> > >            m_InputQueueSizes;
        std::vector<boost::shared_ptr<std::atomic<int> > >            m_OutputQueueSizes;
        std::vector<boost::shared_ptr<boost::mutex> >                m_InputMutexs;
        std::vector<boost::shared_ptr<boost::mutex> >                m_OutputMutexs;
        std::vector<boost::shared_ptr<boost::condition_variable> >   m_InputConds;
        std::vector<boost::shared_ptr<boost::condition_variable> >   m_OutputConds;
        std::vector<boost::shared_ptr<std::queue<void*> > >           m_InputQueues;
        std::vector<boost::shared_ptr<std::queue<void*> > >           m_OutputQueues;
        std::string m_BlockName;
        // Data management functions
        // INPUTS
        voidvec readFromInputQueues();
        void* readFromInputQueue(int inport);
        // OUTPUTS
        void addToOutputQueues(voidvec ProcessedDataVector);
        void addToOutputQueue(void* processedData, int outport);
        void notifyConnectedBlocks();
        // In and Out Process Block
        void block();
        void block_source();
        void block_sink();
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
