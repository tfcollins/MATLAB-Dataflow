// Default functional placeholders
//void blankz(){}
//std::function<void()> blankTemp = blankz;

#include "flowMP.h"

// Constructor
Worker::Worker(std::function<voidvec(voidvec,int*)> ProcessData,
               int numInputs, int numOutputs,
               std::function<void()> FunctionInit, //= blankTemp
               std::function<void()> FunctionCleanup,
               std::string BlockName ) //= blankTemp
{
        m_ProcessData = ProcessData;
        m_FunctionInit = FunctionInit;
        m_FunctionCleanup = FunctionCleanup;

        // Set Parameters
        m_StopThread = false;
        m_NumInputs = numInputs;
        m_NumOutputs = numOutputs;
        m_ProcessData = ProcessData;
        m_FunctionInit = FunctionInit;
        m_FunctionCleanup = FunctionCleanup;
        m_BlockName = BlockName;

        // Extra Benchmarking Parameters
        m_BenchMarkComplete = false;
        m_BenchMarkingCount = 1000;
        m_BenchMarkingStarts = std::vector<std::chrono::high_resolution_clock::time_point>(TIMING_SAMPLES);
        m_BenchMarkingEnds = std::vector<std::chrono::high_resolution_clock::time_point>(TIMING_SAMPLES);

        //Setup Ports
        for(int port=0; port<numInputs; port++)
        {
                boost::shared_ptr<std::atomic<int> > sptr (new std::atomic<int>(0));
                m_InputQueueSizes.push_back(sptr);
                // boost::shared_ptr<boost::mutex> sptr2 (new boost::mutex);
                // m_InputMutexs.push_back(sptr2);
                boost::shared_ptr<boost::condition_variable> sptr3 (new boost::condition_variable);
                m_InputConds.push_back(sptr3);
                // boost::shared_ptr<std::queue<void*> > sptr4 (new std::queue<void*>);
                boost::shared_ptr<moodycamel::BlockingReaderWriterQueue<void*>> sptr4 (new moodycamel::BlockingReaderWriterQueue<void*>);
                m_InputQueues.push_back(sptr4);
        }
        for(int port=0; port<numOutputs; port++)
        {
                boost::shared_ptr<std::atomic<int> > sptr (new std::atomic<int>(0));
                m_OutputQueueSizes.push_back(sptr);
                // boost::shared_ptr<boost::mutex> sptr2 (new boost::mutex);
                // m_OutputMutexs.push_back(sptr2);
                boost::shared_ptr<boost::condition_variable> sptr3 (new boost::condition_variable);
                m_OutputConds.push_back(sptr3);
                // boost::shared_ptr<std::queue<void*> > sptr4 (new std::queue<void*>);
                boost::shared_ptr<moodycamel::BlockingReaderWriterQueue<void*>> sptr4 (new moodycamel::BlockingReaderWriterQueue<void*>);
                m_OutputQueues.push_back(sptr4);
        }

}
/////////////////////////////////////////////////////
///////// INPUTS
/////////////////////////////////////////////////////
voidvec Worker::readFromInputQueues()
{
        voidvec inputs(m_NumInputs);
        // Cycle over ports
        for(int inport=0; inport < m_NumInputs; inport++)
        {
                inputs[inport] = readFromInputQueue(inport);
                // Stop thread
                if (m_StopThread)
                        break;
        }

        return inputs;

}
// Get data from queue
void* Worker::readFromInputQueue(int inport)
{
        // Thread stopped
        if (m_StopThread)
        {
                void* empty;
                return empty;
        }
        // Get New Data
        void *data;
        (*(m_InputQueues[inport])).wait_dequeue(data);
        (*(m_InputQueueSizes[inport]))--;

        return data;
}

/////////////////////////////////////////////////////
///////// OUTPUTS
/////////////////////////////////////////////////////
void Worker::addToOutputQueues(voidvec ProcessedDataVector)
{
        // Cycle over ports
        for(int outport=0; outport < m_NumOutputs; outport++)
                addToOutputQueue(ProcessedDataVector[outport],outport);
}

// Output data to next block
void Worker::addToOutputQueue(void* processedData, int outport)
{
        while ( (*(m_OutputQueueSizes[outport])) > WARNINGQSIZE )
        {
                // Sleeping for this amount is faster than waiting for a signal
                // from downstream blocks
                const int usec = 10;
                boost::this_thread::sleep(boost::posix_time::microseconds(usec));
        }

        // Add data to queue
        (*(m_OutputQueues[outport])).enqueue(processedData);
        (*(m_OutputQueueSizes[outport]))++;
}

// Notify all connected blocks (this being a source to them)
void Worker::notifyConnectedBlocks()
{
        // Cycle over ports
        for(int outport=0; outport < m_NumOutputs; outport++)
                (*(m_OutputConds[outport])).notify_one();
}

// In and Out Process Block
void Worker::block()
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
        // std::cout << "Thread Done: " << m_BlockName << " ID: " << boost::this_thread::get_id() << '\n';

}

// Source Block
void Worker::block_source()
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

        #ifdef BENCHMARKING
        std::cout<<"Benchmark Started\n";
        m_BM_START = std::chrono::high_resolution_clock::now();
        int count = 0;
        #endif

        // Initialize threaded function
        m_FunctionInit();

        while (!m_StopThread)
        {
                //Process Data
                processedData = m_ProcessData(data, &flag);
                if (m_StopThread)
                        break;

                #ifdef BENCHMARKING
                if (count<TIMING_SAMPLES)
                {
                        m_BenchMarkingStarts[count] = std::chrono::high_resolution_clock::now();
                        count++;
                }
                #endif

                //Output Data
                if (flag>0)
                        addToOutputQueues(processedData);

        }
        std::cout<<"Source block finished\n";

        // Cleanup after threaded function
        m_FunctionCleanup();

        // Notify when thread completes
        // std::cout << "Thread Done: " << m_BlockName << " ID: " << boost::this_thread::get_id() << '\n';

}
// Sink Block
void Worker::block_sink()
{
    #ifdef UNIX
        prctl(PR_SET_NAME,m_BlockName.c_str(),0,0,0);
    #endif

        voidvec data;
        voidvec processedData;
        int flag = 0;

        #ifdef BENCHMARKING
        int count = 0;
        #endif

        // Initialize threaded function
        m_FunctionInit();

        while (!m_StopThread)
        {
                //Read Data
                data = readFromInputQueues();
                if (m_StopThread)
                        break;

                #ifdef BENCHMARKING
                if (count<TIMING_SAMPLES)
                    m_BenchMarkingEnds[count] = std::chrono::high_resolution_clock::now();
                #endif

                //Process Data
                processedData = m_ProcessData(data,&flag);

                #ifdef BENCHMARKING
                count++;
                if (count==m_BenchMarkingCount)
                {
                  m_BM_END = std::chrono::high_resolution_clock::now();
                  std::cout<<"Benchmark Done\n";
                  m_BenchMarkComplete = true;
                }
                #endif
        }
        // std::cout<<"Sink block finished\n";

        // Cleanup after threaded function
        m_FunctionCleanup();

        // Notify when thread completes
        // std::cout << "Thread Done: " << m_BlockName << " ID: " << boost::this_thread::get_id() << '\n';

}
// Spawn threads for given block type
void Worker::run()
{
        boost::thread m_BlockThread(&Worker::block, this);
}
void Worker::run_source()
{
        boost::thread m_BlockThread(&Worker::block_source, this);
}
void Worker::run_sink()
{
        boost::thread m_BlockThread(&Worker::block_sink, this);
}

// Connect blocks together
void connect(Worker &aBlock, int outport, Worker &bBlock, int inport)
{
	// // Map mutexes
	// bBlock.m_InputMutexs[inport] = aBlock.m_OutputMutexs[outport];
	// // Map conditionals
	// bBlock.m_InputConds[inport] = aBlock.m_OutputConds[outport];
    // Map atomics
	bBlock.m_InputQueueSizes[inport] = aBlock.m_OutputQueueSizes[outport];
	// Map queues
	bBlock.m_InputQueues[inport] = aBlock.m_OutputQueues[outport];
}
