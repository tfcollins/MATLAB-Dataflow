// Dataflow Backbone
// author: Travis Collins (travisfcollins@gmail.com)
//
// Last Update: 8/20/2015

#include <iostream>
#include <queue>
#include <boost/thread.hpp>
#include <atomic>
#include <functional>
#include <boost/scoped_ptr.hpp>
#include <vector>
#include <time.h>
#ifdef UNIX
  #include <sys/prctl.h>
#endif

// If queue grows large warn since memory may be fully consumed eventually
#define WARNINGQSIZE 300

// Default functional placeholders
//void blankz(){}
//std::function<void()> blankTemp = blankz;

template<class T>
void* consumeOut(T in)
{
 return (void*) in;
}

// Type declarations
typedef std::vector<void*> voidvec;
typedef std::vector<void*> OUTPUTS;
typedef std::vector<void*> INPUTS;

// Class to be threaded
class Worker
{
public:
    // Processing/Setup/Cleanup functions
    std::function<voidvec(voidvec,int*)> m_ProcessData;
    std::function<void()> m_FunctionInit;
    std::function<void()> m_FunctionCleanup;

    // User defined parameters
    int m_NumInputs;
    int m_NumOutputs;

    // Constructor
    Worker(std::function<voidvec(voidvec,int*)> ProcessData,
            int numInputs, int numOutputs,
           std::function<void()> FunctionInit,//= blankTemp
           std::function<void()> FunctionCleanup,
           std::string BlockName ) ://= blankTemp
    m_StopThread(false),
    m_NumInputs(numInputs),
    m_NumOutputs(numOutputs),
            m_ProcessData(ProcessData),
            m_FunctionInit(FunctionInit),
            m_FunctionCleanup(FunctionCleanup),
            m_BlockName(BlockName)
    {
        //Setup Ports
        for(int port=0;port<numInputs;port++)
        {
            boost::shared_ptr<std::atomic<int>> sptr (new std::atomic<int>(0));
            m_InputQueueSizes.push_back(sptr);
            boost::shared_ptr<boost::mutex> sptr2 (new boost::mutex);
            m_InputMutexs.push_back(sptr2);
            boost::shared_ptr<boost::condition_variable> sptr3 (new boost::condition_variable);
            m_InputConds.push_back(sptr3);
            boost::shared_ptr<std::queue<void*>> sptr4 (new std::queue<void*>);
            m_InputQueues.push_back(sptr4);
        }
        for(int port=0;port<numOutputs;port++)
        {
            boost::shared_ptr<std::atomic<int>> sptr (new std::atomic<int>(0));
            m_OutputQueueSizes.push_back(sptr);
            boost::shared_ptr<boost::mutex> sptr2 (new boost::mutex);
            m_OutputMutexs.push_back(sptr2);
            boost::shared_ptr<boost::condition_variable> sptr3 (new boost::condition_variable);
            m_OutputConds.push_back(sptr3);
            boost::shared_ptr<std::queue<void*>> sptr4 (new std::queue<void*>);
            m_OutputQueues.push_back(sptr4);
        }

    }

    // Parameters
    std::atomic<bool>                              m_StopThread;
    boost::thread                                  m_BlockThread;
    // MIMO Parameters
    std::vector<boost::shared_ptr<std::atomic<int>>>            m_InputQueueSizes;
    std::vector<boost::shared_ptr<std::atomic<int>>>            m_OutputQueueSizes;
    std::vector<boost::shared_ptr<boost::mutex>>                m_InputMutexs;
    std::vector<boost::shared_ptr<boost::mutex>>                m_OutputMutexs;
    std::vector<boost::shared_ptr<boost::condition_variable>>   m_InputConds;
    std::vector<boost::shared_ptr<boost::condition_variable>>   m_OutputConds;
    std::vector<boost::shared_ptr<std::queue<void*>>>           m_InputQueues;
    std::vector<boost::shared_ptr<std::queue<void*>>>           m_OutputQueues;
    std::string m_BlockName;

    //Destructor
    ~Worker()
    {
      for(uint port=0; port<m_InputConds.size(); port++)
      {
        //(*(m_InputConds[port])).notify_all();
        //(*(m_InputMutexs[port])).unlock();
        //(m_InputMutexs[port]).reset();
      }

      for(uint port=0; port<m_OutputConds.size(); port++)
      {
        m_OutputConds[port].reset();
        //(*(m_OutputMutexs[port])).unlock();
        (m_OutputMutexs[port]).reset(); //SEG FAULTING HERE
      }
    }

// Data management functions

/////////////////////////////////////////////////////
///////// INPUTS
/////////////////////////////////////////////////////
voidvec readFromInputQueues()
{
    voidvec inputs(m_NumInputs);
    // Cycle over ports
    for(int inport=0; inport < m_NumInputs ; inport++)
    {
        inputs[inport] = readFromInputQueue(inport);
        // Stop thread
        if (m_StopThread)
          break;
    }

    return inputs;

}
// Get data from queue
void* readFromInputQueue(int inport)
{

    // Wait for data
    if (*(m_InputQueueSizes[inport]) == 0){
        // Wait for signal
        //boost::unique_lock<boost::mutex> lock(*(m_InputMutexs[inport]));
        boost::mutex::scoped_lock lock(*(m_InputMutexs[inport]));

        do {(*(m_InputConds[inport])).wait(lock);}//Wait will tell the lock to unlock
        while ((*(m_InputQueueSizes[inport]) == 0) && (!m_StopThread));//If we wake up, make sure we have data to work with
    }
    else if ((*(m_InputQueueSizes[inport]))>WARNINGQSIZE)
        std::cout<<"Input Queue Size: "<<(*(m_InputQueueSizes[inport]))<<" | "<< m_BlockName << std::endl;

    // Thread stopped
    if (m_StopThread)
    {
      void* empty;
      return empty;
    }

    // When data is ready read it off queue
    //boost::lock_guard<boost::mutex> lock(*(m_InputMutexs[inport]));
    boost::mutex::scoped_lock lock(*(m_InputMutexs[inport]));

    void* data = (*(m_InputQueues[inport])).front();
    (*(m_InputQueues[inport])).pop();

    // Update queue size atomic
    (*(m_InputQueueSizes[inport]))--;

    return data;
}

/////////////////////////////////////////////////////
///////// OUTPUTS
/////////////////////////////////////////////////////
void addToOutputQueues(voidvec ProcessedDataVector)
{
    // Cycle over ports
    for(int outport=0; outport < m_NumOutputs ; outport++)
        addToOutputQueue(ProcessedDataVector[outport],outport);
}

// Output data to next block
void addToOutputQueue(void* processedData, int outport)
{
    // Add data to queue
    boost::lock_guard<boost::mutex> lock(*(m_OutputMutexs[outport]));
    (*(m_OutputQueues[outport])).push(processedData);
    (*(m_OutputQueueSizes[outport]))++;

    // Notify next block
    (*(m_OutputConds[outport])).notify_one();

}

// Notify all connected blocks (this being a source to them)
void notifyConnectedBlocks()
{
    // Cycle over ports
    for(int outport=0; outport < m_NumOutputs ; outport++)
        (*(m_OutputConds[outport])).notify_one();
}

// In and Out Process Block
void block()
{
    #ifdef UNIX
    prctl(PR_SET_NAME,m_BlockName.c_str(),0,0,0);
    #endif
    voidvec data;
    voidvec processedData;
    int flag = 0;

    // Initialize threaded function
    m_FunctionInit();

    while (!m_StopThread)
    {
        //Read Data
        data = readFromInputQueues();
        if (m_StopThread)
          break;

        //Process Data
        processedData = m_ProcessData(data,&flag);
        //Output Data
        if (flag>0)
            addToOutputQueues(processedData);
    }
    // Cleanup after threaded function
    m_FunctionCleanup();

    // Notify when thread completes
    std::cout << "Thread Done: " << m_BlockName << " ID: " << boost::this_thread::get_id() << '\n';

}

// Source Block
void block_source()
{
    #ifdef UNIX
    prctl(PR_SET_NAME,m_BlockName.c_str(),0,0,0);
    #endif

    // Delay startup, to make sure all blocks have started
    int usec = 1000;
    boost::this_thread::sleep(boost::posix_time::microseconds(usec));

    voidvec data;
    voidvec processedData;
    int flag = 0;

    // Initialize threaded function
    m_FunctionInit();

    while (!m_StopThread)
    {
        //Process Data
        processedData = m_ProcessData(data, &flag);
        if (m_StopThread)
          break;

        //Output Data
        if (flag>0)
            addToOutputQueues(processedData);

    }
    std::cout<<"Source block finished\n";

    // Cleanup after threaded function
    m_FunctionCleanup();

    // Notify when thread completes
    std::cout << "Thread Done: " << m_BlockName << " ID: " << boost::this_thread::get_id() << '\n';

}
// Sink Block
void block_sink()
{
    #ifdef UNIX
    prctl(PR_SET_NAME,m_BlockName.c_str(),0,0,0);
    #endif

    voidvec data;
    voidvec processedData;
    int flag = 0;

    // Initialize threaded function
    m_FunctionInit();

    while (!m_StopThread)
    {
        //Read Data
        data = readFromInputQueues();
        if (m_StopThread)
          break;

        //Process Data
        processedData = m_ProcessData(data,&flag);
    }
    std::cout<<"Sink block finished\n";

    // Cleanup after threaded function
    m_FunctionCleanup();

    // Notify when thread completes
    std::cout << "Thread Done: " << m_BlockName << " ID: " << boost::this_thread::get_id() << '\n';

}
// Spawn threads for given block type
void run()
{
    boost::thread m_BlockThread(&Worker::block, this);
}
void run_source()
{
    boost::thread m_BlockThread(&Worker::block_source, this);
}
void run_sink()
{
    boost::thread m_BlockThread(&Worker::block_sink, this);
}
};

/*
// Connect blocks together
void ConnectBlocks(Worker &aBlock, int outport, Worker &bBlock, int inport)
{
	// Map mutexes
	bBlock.m_InputMutexs[inport] = aBlock.m_OutputMutexs[outport];
	// Map conditionals
	bBlock.m_InputConds[inport] = aBlock.m_OutputConds[outport];
	// Map queues
	bBlock.m_InputQueues[inport] = aBlock.m_OutputQueues[outport];
	// Map atomics
	bBlock.m_InputQueueSizes[inport] = aBlock.m_OutputQueueSizes[outport];
}
*/


/*
// ------Helper Functions------
// Non class functions
template <typename IN, typename OUT>
void addToQueue(IN data, Worker<IN,OUT> &aBlock)
{
    // Add data to queue
    boost::unique_lock<boost::mutex> lock(*aBlock.m_InputMutex);
    (*aBlock.m_InputQueue).push(data);
    *aBlock.m_InputQueueSize = (*aBlock.m_InputQueueSize) + 1;

    // Notify next block
    (*aBlock.m_InputCond).notify_one();
}


template <typename IN, typename OUT>
OUT readFromQueue(Worker<IN,OUT> &aBlock)
{
    // Wait for data
    if (*aBlock.m_OutputQueueSize == 0) {
        boost::unique_lock<boost::mutex> lock(*aBlock.m_OutputMutex);
        do (*aBlock.m_OutputCond).wait(lock);
        while (*aBlock.m_OutputQueueSize == 0);
    }

    // When data is ready read it off queue
    boost::unique_lock<boost::mutex> lock(*aBlock.m_OutputMutex);
    OUT data = (*aBlock.m_OutputQueue).front();
    (*aBlock.m_OutputQueue).pop();
    // Update queue size atomic
    *aBlock.m_OutputQueueSize = *aBlock.m_OutputQueueSize - 1;
    return data;

}
 */
