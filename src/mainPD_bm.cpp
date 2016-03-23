// Needed for benchmarking
#include <boost/timer/timer.hpp>

// Backend dataflow
#include "flowMP.h"
#include "utils.h"
#include "graph.h"

#ifdef _WIN32 // Windows still not fully working (boost problems)
    #include <Windows.h>
#endif

extern boost::timer::cpu_times times;
extern long counter;

// // Block header files
// #include "CheckCRC.h"
// #include "GenCRC.h"

// // Library header files
// #include "RX_terminate.h"
// #include "RX_initialize.h"

// Block definitions
#include "blocks_bm.h"

using namespace std;

void do_nothing(){}

double Eval_Time()
{
    // Calculate time to do one pass
    boost::timer::nanosecond_type elapsed(times.wall);

    // double nanoSecondsPerSecond = 1000000000;
    // std::cout << "Function Pass Delay: " << (((double)elapsed)/((double)maxCount*nanoSecondsPerSecond)) << std::endl;

    double nanoSecondsPerHandover = (((double)elapsed)/((double)maxCount));

    std::cout << "Function Pass Delay: " << nanoSecondsPerHandover << std::endl;

    return nanoSecondsPerHandover;

}


//MAIN
int main()
{
        int runs = 10;
        std::vector<double> runTimes(runs);

        for (int i=0; i<runs; i++)
        {
                start = false; counter = 1;// Reset test globals

                Worker block1(Producer, 0, 1, do_nothing, do_nothing, "Producer");
                Worker block2(Consumer, 1, 0, do_nothing, do_nothing, "Consumer");

                //// Connect blocks together (Port numbers start with 0) ////
                connect(block1, 0, block2, 0);

                //// Create graph and add blocks ////
                Graph Benchmark("CRC Test Example");
                Benchmark.Blocks = {&block1,&block2};

                // Label Sources and Sinks (Strings from block thread names)
                Benchmark.Sources = {"Producer"};
                Benchmark.Sinks = {"Consumer"};

                //// Run Graph ////
                double SimDuration = 20; // set SimDuration<=0 for continuous run
                cout<<"Starting up (purposely delayed)\n";
                Benchmark.run(SimDuration); // Turn blocks on (blocking)

                //delete block1, block2, Benchmark;

                runTimes[i] = Eval_Time();
                std::cout<<"---------------\n";
        }

        double sum = std::accumulate(runTimes.begin(), runTimes.end(), 0.0);
        double mean = sum / runTimes.size();
        std::cout<<"Mean Time: "<<mean<<std::endl;

	double sq_sum = std::inner_product(runTimes.begin(), runTimes.end(), runTimes.begin(), 0.0);
	double stddev = std::sqrt(sq_sum / runTimes.size() - mean * mean);
        std::cout<<"STDDev: "<<stddev<<std::endl;

        return 0;
}
