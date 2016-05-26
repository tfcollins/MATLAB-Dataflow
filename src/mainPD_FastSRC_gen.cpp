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
#include "ModSignalDump.h"

// Library header files
#include "FastSRC_terminate.h"
#include "FastSRC_initialize.h"

// Block definitions
#include "blocks_gen.h"

using namespace std;

int main()
{

    // Create instances of blocks
    Worker block1(GenSignalBLK, 0, 1, FastSRC_initialize, FastSRC_terminate, "GenSignal Source");
    Worker block2(ModSignalDumpBLK, 1, 0, FastSRC_initialize, FastSRC_terminate, "ModSignalDump Sink");

    //// Connect blocks together (Port numbers start with 0) ////
    connect(block1, 0, block2, 0);

    //// Create graph and add blocks ////
    Graph FlowGraph("MyGraph");
    FlowGraph.Blocks = {&block1, &block2};

    // Enable Benchmarking
    block2.m_BenchMarkingCount = 1000000;

    // Label Sources and Sinks (Strings from block thread names)
    FlowGraph.Sources = {"GenSignal Source"};
    FlowGraph.Sinks = {"ModSignalDump Sink"};

    //// Run Graph ////
    cout<<"MAIN>> Type:  top -H -p `pidof FastSRC` in a terminal to view thread utilization";
    double SimDuration = 60;// set SimDuration<=0 for continuous run
    cout<<"Starting up (purposely delayed)";
    FlowGraph.run(SimDuration);// Turn blocks on

    return 0;

}
