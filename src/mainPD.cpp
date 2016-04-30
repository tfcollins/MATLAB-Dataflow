// Backend dataflow
#include "flowMP.h"
#include "utils.h"
#include "graph.h"

#ifdef _WIN32 // Windows still not fully working (boost problems)
    #include <Windows.h>
#endif

// Block header files
#include "CheckCRC.h"
#include "GenCRC.h"

// Library header files
#include "RX_terminate.h"
#include "RX_initialize.h"

// Block definitions
#include "blocks_gen.h"

using namespace std;

//MAIN
int main()
{
    // Create instances of blocks
    // Template (Functional_Name, Inputs, Outputs, Initialization_function, Termination_function, Thread_Name)
    // Functional_Name: functional defined in blocks.h
    // Inputs/Outputs: integers number of ports for each block
    // Initialization_function: Function name produced by MATLAB coder to be called before function is called
    // Termination_function: Function name produced by MATLAB coder to be called after function is never to be called again
    // Thread_Name: String that will help monitor different threads is programs like 'top'

    Worker block1(GenCRCBLK, 0, 1, RX_initialize, RX_terminate, "CRC Source");
    Worker block2(CheckCRCBLK,   1, 0, RX_initialize, RX_terminate, "CRC Sink");

    //// Connect blocks together (Port numbers start with 0) ////
    connect(block1, 0, block2, 0);

    //// Create graph and add blocks ////
    Graph Receiver("CRC Test Example");
    Receiver.Blocks = {&block1,&block2};

    // Label Sources and Sinks (Strings from block thread names)
    Receiver.Sources = {"CRC Source"};
    Receiver.Sinks = {"CRC Sink"};

    //// Run Graph ////
    cout<<"MAIN>> Type: 'top -H -p `pidof RX`' in a terminal to view thread utilization\n";
    double SimDuration = 60;// set SimDuration<=0 for continuous run
    cout<<"Starting up (purposely delayed)\n";
    Receiver.run(SimDuration);// Turn blocks on


    return 0;
}
