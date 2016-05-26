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
#include "DumpSignal.h"

// Library header files
#include "SlowSRC_terminate.h"
#include "SlowSRC_initialize.h"

// Block definitions
#include "blocks_gen.h"

using namespace std;

int main()
{

    // Create instances of blocks
    Worker block1(GenSignalBLK, 0, 1, SlowSRC_initialize, SlowSRC_terminate, "GenSignal Source");
    Worker block2(DumpSignalBLK, 1, 0, SlowSRC_initialize, SlowSRC_terminate, "DumpSignal Sink");

    //// Connect blocks together (Port numbers start with 0) ////
    connect(block1, 0, block2, 0);

    //// Create graph and add blocks ////
    Graph FlowGraph("MyGraph");
    FlowGraph.Blocks = {&block1, &block2};

    // Enable Benchmarking
    block2.m_BenchMarkingCount = 1000000;

    // Label Sources and Sinks (Strings from block thread names)
    FlowGraph.Sources = {"GenSignal Source"};
    FlowGraph.Sinks = {"DumpSignal Sink"};

    //// Run Graph ////
    cout<<"MAIN>> Type:  top -H -p `pidof SlowSRC` in a terminal to view thread utilization";
    double SimDuration = 60;// set SimDuration<=0 for continuous run
    cout<<"Starting up (purposely delayed)";
    FlowGraph.run(SimDuration);// Turn blocks on

    return 0;

}
