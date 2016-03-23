// Utilities

//Defines
#define ONESECOND 1000000

// Blank data, used for sink return calls
void* none;

// Connect blocks together
void connect(Worker &aBlock, int outport, Worker &bBlock, int inport)
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

// Consume Outputs to create void* output vector
template<typename U>
void Push_back(voidvec &items, U item)
{
        items.push_back((void*)item);
}

template<typename U, typename ...Args>
void Push_back(voidvec &items, U item, Args ...args)
{
        items.push_back((void*)item);
        Push_back( items, args... );
}

template<typename... Args>
voidvec SendOut(Args... Fargs)
{
        voidvec vec;
        Push_back(vec, Fargs...);
        return vec;
}

// Shutdown blocks
template<typename U>
void Shutdown(U block)
{
    (*block).m_StopThread = true;
    int usec = 100;
    boost::this_thread::sleep(boost::posix_time::microseconds(usec));
    (*block).m_BlockThread.join();
}

template<typename U, typename ...Args>
void Shutdown(U block, Args ...args)
{
    (*block).m_StopThread = true;
    (*block).m_BlockThread.join();
    int usec = 100;
    boost::this_thread::sleep(boost::posix_time::microseconds(usec));
    Shutdown( args... );
}
template<typename ...Args>
void StopSim(Args ...args)
{
    // std::cout<<"INFO>> ------SHUTTING DOWN------\n";
    // std::cout<<"INFO>> Waiting for threads to quit\n";
    // std::cout<<"INFO>> Ignore errors that may follow\n";

    try
        {Shutdown(args...);}
    catch(...)
    {}
}
// Simulation sleep
void SimDuration(double duration)
{
    // std::cout<<"INFO>> Running simulation for "<<duration<<" seconds\n";
    // std::cout<<"INFO>> Main thread sleeping\n";
/*
    for(int j=0;j<duration;j++)
		{
			int usec = ONESECOND;
			boost::this_thread::sleep(boost::posix_time::microseconds(usec));
		}
*/
	int usec = ONESECOND*duration;
	boost::this_thread::sleep(boost::posix_time::microseconds(usec));


    // std::cout<<"INFO>> Main thread done sleeping\n";
}

// [EOF]
