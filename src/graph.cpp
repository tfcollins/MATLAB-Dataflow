
#include "graph.h"
#include "flowMP.h"

//Defines
#define ONESECOND 1000000

// Constructor
Graph::Graph(std::string Name)
{
    m_Name = Name;
    m_BenchMarking = false;
}

// Destructor
Graph::~Graph()
{}

// Run graph
void Graph::run(double duration)
{
        // Setup sink(s) first
        for (int b=0; b<(int)Blocks.size(); b++)
                for (int n=0; n<(int)Sinks.size(); n++)
                        if (((*Blocks[b]).m_BlockName).compare(Sinks[n])==0)
                        {
                                //std::cout<<(*Blocks[b]).m_BlockName<<" is a sink\n";
                                (*Blocks[b]).m_BenchMarking = m_BenchMarking;
                                (*Blocks[b]).run_sink();
                        }

        // Setup in and out blocks
        for (int b=0; b<(int)Blocks.size(); b++)
                for (int n=0; n<(int)Sinks.size(); n++)
                        if ((((*Blocks[b]).m_BlockName).compare(Sinks[n])!=0)&&
                            (((*Blocks[b]).m_BlockName).compare(Sources[n])!=0))
                        {
                                //std::cout<<(*Blocks[b]).m_BlockName<<" is a block\n";
                                (*Blocks[b]).m_BenchMarking = m_BenchMarking;
                                (*Blocks[b]).run();
                        }

        // Setup Source(s)
        for (int b=0; b<(int)Blocks.size(); b++)
                for (int n=0; n<(int)Sinks.size(); n++)
                        if (((*Blocks[b]).m_BlockName).compare(Sources[n])==0)
                        {
                                //std::cout<<(*Blocks[b]).m_BlockName<<" is a source\n";
                                (*Blocks[b]).m_BenchMarking = m_BenchMarking;
                                (*Blocks[b]).run_source();
                        }

        // Setup Timers
        clock_t init, final;//, mid;
        init = clock();

        // Run simulation for awhile
        if (duration>0)
                SimDuration(duration);  //Seconds
        else
        {
                std::cout<<"INFO>> Press Enter to stop\n";
                std::cin.ignore();
        }
        // Stop Timers
        final = clock() - init;
        std::cout << "INFO>> Test duration: " << (double)final / ((double)CLOCKS_PER_SEC) << " Seconds" << std::endl;

        ////// BENCHMARKING //////
        #ifdef BENCHMARKING
            int lastBlock = Blocks.size();
            // Get earliest timer of all sources
            std::chrono::high_resolution_clock::time_point earliest_start, lastest_end, zero;
            earliest_start = std::chrono::high_resolution_clock::now();
            for (int b=0; b<(int)Blocks.size(); b++)
            {
              // Check if set
              std::chrono::duration<double> val = ((*Blocks[b]).m_BM_START) - zero;
              if (val.count()==0)
                continue;

              std::chrono::duration<double> diff = ((*Blocks[b]).m_BM_START) - earliest_start;
              if (diff.count()<0)
              earliest_start = (*Blocks[b]).m_BM_START;
            }
            // Get lastest endtime of all timers
            lastest_end = earliest_start;
            for (int b=0; b<(int)Blocks.size(); b++)
            {
              // Check if set
              std::chrono::duration<double> val = ((*Blocks[b]).m_BM_START) - zero;
              if (val.count()==0)
                continue;

              std::chrono::duration<double> diff = ((*Blocks[b]).m_BM_END) - lastest_end;
              if (diff.count()>0)
              lastest_end = (*Blocks[b]).m_BM_END;
            }
            //
            earliest_start = (*Blocks[0]).m_BM_START;
            lastest_end = (*Blocks[lastBlock-1]).m_BM_END;

            // Print result
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::nanoseconds>( lastest_end - earliest_start ).count();
            std::cout << elapsed_seconds << " ns\n";

            // Save runtime to file
            std::ofstream myfileRuntime;
            std::string str;
            str.append("RunDurationFFTBlocks_");
            str.append(std::to_string(lastBlock-2));
            str.append(".txt");
            myfileRuntime.open(str);
            myfileRuntime << elapsed_seconds << " ns\n";
            myfileRuntime.close();

            // Get Time Between Blocks message passes
            std::vector<std::chrono::high_resolution_clock::time_point> BenchMarkingStarts = (*Blocks[0]).m_BenchMarkingStarts;
            std::vector<std::chrono::high_resolution_clock::time_point> BenchMarkingEnds = (*Blocks[lastBlock-1]).m_BenchMarkingEnds;
            std::chrono::high_resolution_clock::time_point zeroHR;
            std::cout<<"\n\nEvaluating Block Timings\n";
            std::ofstream myfile;
            myfile.open("DataPassingTimes.txt");
            myfile << "Pass Time (ns)\n";
            for (int k=0;k<TIMING_SAMPLES;k++)
            {
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>( BenchMarkingEnds[k] - BenchMarkingStarts[k] ).count();
                auto s = std::chrono::duration_cast<std::chrono::nanoseconds>( BenchMarkingStarts[k] - zeroHR ).count();
                auto e = std::chrono::duration_cast<std::chrono::nanoseconds>( BenchMarkingEnds[k] - zeroHR ).count();
                // Only output processed data
                if (duration>0)
                    myfile << duration << " " << s << " " << e << "\n";
            }
            myfile.close();
            std::cout<<"Done Evaluating Block Timings\n";
        #endif
        //////  //////

        // Shutdown
        shutdown();

}

// Simulation sleep
void Graph::SimDuration(double duration)
{
    #ifdef BENCHMARKING
        std::cout<<"In Benchmarking Mode, Sim Time Ignored\n";
        int usec = ONESECOND;
        // for(int i=0; i<duration; i++)
        while (true)
        {
                // Wait In 1 Second increments
                boost::this_thread::sleep(boost::posix_time::microseconds(usec));
                // Check if benchmark completed
                for (int b=0; b<(int)Blocks.size(); b++)
                {
                        if ((*Blocks[b]).m_BenchMarkComplete)
                                return;
                }
        }
    #else
        int usec = ONESECOND*duration;
        boost::this_thread::sleep(boost::posix_time::microseconds(usec));
    #endif
}

// Stop graph
void Graph::shutdown()
{
        // Telling all threads to stop when signaled
        for (int b=0; b<(int)Blocks.size(); b++)
        {
                (*Blocks[b]).m_StopThread = true;
        }
        // Signaling blocks to wakeup and shutdown
        for (int b=0; b<(int)Blocks.size(); b++)
        {
                (*Blocks[b]).notifyConnectedBlocks();
        }
        // Join all threads
        for (int b=0; b<(int)Blocks.size(); b++)
        {
                (*Blocks[b]).m_BlockThread.join();
        }
        // Wait for all to stop
        int usec = 4000000;
        boost::this_thread::sleep(boost::posix_time::microseconds(usec));
}
