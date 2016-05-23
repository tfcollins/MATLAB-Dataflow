// Backend dataflow
#include "flowMP.h"
#include "utils.h"
#include "graph.h"
#include <fstream>
#ifdef _WIN32 // Windows still not fully working (boost problems)
     #include <Windows.h>
#endif

// Block Header Files
#include "GenCRC.h"
#include "CheckCRC.h"

// Library header files
#include "RX_terminate.h"
#include "RX_initialize.h"

// Block definitions
#include "blocks_gen.h"

using namespace std;

int main()
{

    // Create instances of blocks
    Worker block1(GenCRCBLK, 0, 1, RX_initialize, RX_terminate, "GenCRC Source");
    Worker block2(CheckCRCBLK, 1, 0, RX_initialize, RX_terminate, "CheckCRC Sink");

    //// Connect blocks together (Port numbers start with 0) ////
    connect(block1, 0, block2, 0);

    //// Create graph and add blocks ////
    Graph FlowGraph("MyGraph");
    FlowGraph.Blocks = {&block1, &block2};

    // Enable Benchmarking
    block2.m_BenchMarkingCount = 100;

    // Label Sources and Sinks (Strings from block thread names)
    FlowGraph.Sources = {"GenCRC Source"};
    FlowGraph.Sinks = {"CheckCRC Sink"};

    //// Run Graph ////
    cout<<"MAIN>> Type:  top -H -p `pidof RX` in a terminal to view thread utilization";
    double SimDuration = 30;// set SimDuration<=0 for continuous run
    cout<<"Starting up (purposely delayed)";
    FlowGraph.run(SimDuration);// Turn blocks on

    return 0;

}
