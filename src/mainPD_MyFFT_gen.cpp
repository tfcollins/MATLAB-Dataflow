// Backend dataflow
#include "flowMP.h"
#include "utils.h"
#include "graph.h"
#include <fstream>
#ifdef _WIN32 // Windows still not fully working (boost problems)
     #include <Windows.h>
#endif

// Block Header Files
#include "GenSignal.h"
#include "FFTSignal.h"
#include "FFTSignal.h"
#include "DumpSignal.h"

// Library header files
#include "MyFFT_terminate.h"
#include "MyFFT_initialize.h"

// Block definitions
#include "blocks_gen.h"

using namespace std;

int main()
{

    // Create instances of blocks
    Worker block1(GenSignalBLK, 0, 1, MyFFT_initialize, MyFFT_terminate, "GenSignal Source");
    Worker block2(FFTSignalBLK, 1, 1, MyFFT_initialize, MyFFT_terminate, "FFTSignal Block");
    Worker block3(FFTSignalBLK, 1, 1, MyFFT_initialize, MyFFT_terminate, "FFTSignal Block");
    Worker block4(DumpSignalBLK, 1, 0, MyFFT_initialize, MyFFT_terminate, "DumpSignal Sink");

    //// Connect blocks together (Port numbers start with 0) ////
    connect(block1, 0, block2, 0);
    connect(block2, 0, block3, 0);
    connect(block3, 0, block4, 0);

    //// Create graph and add blocks ////
    Graph FlowGraph("MyGraph");
    FlowGraph.Blocks = {&block1, &block2, &block3, &block4};

    // Enable Benchmarking
    block4.m_BenchMarkingCount = 100;

    // Label Sources and Sinks (Strings from block thread names)
    FlowGraph.Sources = {"GenSignal Source"};
    FlowGraph.Sinks = {"DumpSignal Sink"};

    //// Run Graph ////
    cout<<"MAIN>> Type:  top -H -p `pidof MyFFT` in a terminal to view thread utilization";
    double SimDuration = 30;// set SimDuration<=0 for continuous run
    cout<<"Starting up (purposely delayed)";
    FlowGraph.run(SimDuration);// Turn blocks on

    return 0;

}
