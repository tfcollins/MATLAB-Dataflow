
// Graph Class
class Graph
{
public:
    // Constructor
    Graph(std::string Name):
    m_Name(Name)
    {}
    //Parameters
    std::string m_Name;
    std::vector<Worker*> Blocks;
    std::vector<std::string> Sources;
    std::vector<std::string> Sinks;

    // Destructor
    //~Graph();



// Run graph
void run(double duration)
{
    // Setup sink(s) first
    for (int b=0;b<(int)Blocks.size();b++)
        for (int n=0;n<(int)Sinks.size();n++)
            if (((*Blocks[b]).m_BlockName).compare(Sinks[n])==0)
            {
                //std::cout<<(*Blocks[b]).m_BlockName<<" is a sink\n";
                (*Blocks[b]).run_sink();
            }

    // Setup in and out blocks
    for (int b=0;b<(int)Blocks.size();b++)
        for (int n=0;n<(int)Sinks.size();n++)
            if ((((*Blocks[b]).m_BlockName).compare(Sinks[n])!=0)&&
               (((*Blocks[b]).m_BlockName).compare(Sources[n])!=0))
            {
                //std::cout<<(*Blocks[b]).m_BlockName<<" is a block\n";
                (*Blocks[b]).run();
            }

    // Setup Source(s)
    for (int b=0;b<(int)Blocks.size();b++)
        for (int n=0;n<(int)Sinks.size();n++)
            if (((*Blocks[b]).m_BlockName).compare(Sources[n])==0)
            {
                //std::cout<<(*Blocks[b]).m_BlockName<<" is a source\n";
                (*Blocks[b]).run_source();
            }

    // Setup Timers
    clock_t init, final, mid;
    init=clock();

    // Run simulation for awhile
    if (duration>0)
        SimDuration(duration); //Seconds
    else
    {
        std::cout<<"INFO>> Press Enter to stop\n";
        std::cin.ignore();
    }

    // Stop Timers
    final=clock()-init;
    // std::cout << "INFO>> Test duration: " << (double)final / ((double)CLOCKS_PER_SEC) << " Seconds" << std::endl;

    // Shutdown
    shutdown();

}

// Stop graph
void shutdown()
{
    // std::cout<<"INFO>> ------SHUTTING DOWN------\n";
    // std::cout<<"INFO>> Waiting for threads to quit\n";
    // std::cout<<"INFO>> Ignore errors that may follow\n";
    // std::cout<<"Blocks to shutdown: "<<Blocks.size()<<std::endl;
  //   for (int b=0;b<Blocks.size();b++)
  //   {
	//      std::cout<<"Pre try\n";
  //       try
  //       {
	//     std::cout<<"Stoping thread\n";
  //           (*Blocks[b]).m_StopThread = true;
	//     std::cout<<"Stop flag set\n";
  //           (*Blocks[b]).m_BlockThread.join();
  //           int usec = 1000;
  //           boost::this_thread::sleep(boost::posix_time::microseconds(usec));
  //       }
  //       catch (...)
  //       {
	// 	std::cout<<"Exception on block: "<<b<<std::endl;
	// }
  //   }

    // Telling all threads to stop when signaled
    for (int b=0;b<(int)Blocks.size();b++)
    {
        (*Blocks[b]).m_StopThread = true;
    }
    // Signaling blocks to wakeup and shutdown
    for (int b=0;b<(int)Blocks.size();b++)
    {
        (*Blocks[b]).notifyConnectedBlocks();
    }
    // Join all threads
    for (int b=0;b<(int)Blocks.size();b++)
    {
        (*Blocks[b]).m_BlockThread.join();
    }

    int usec = 4000000;
    boost::this_thread::sleep(boost::posix_time::microseconds(usec));
    // std::cout<<"Block stopping complete\n";
}
};
