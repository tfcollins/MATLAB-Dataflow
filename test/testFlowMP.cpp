#include "flowMP.h"
#include "utils.h"
#include "graph.h"
//#ifdef _WIN32
//    #include <Windows.h>
//#endif

using namespace std;

//***Block Definitions***//

/////////////////////////////////////////////////
OUTPUTS Source(INPUTS input, int *flag)
{
    // Slow this down so we don't max out the core
    int usec = 1000;
    boost::this_thread::sleep(boost::posix_time::microseconds(usec));

    // Setup output
    double *data = new double[10];

    // Process
    for(int index=0; index<10; index++)
        data[index] = index;

    //Enable pass to next block
    *flag = 1;
    return SendOut(data);
}
/////////////////////////////////////////////////
OUTPUTS Sink(INPUTS input, int *flag)
{
    // Get input
    double *data = (double*) input[0];

    // Process
    // for(int index=0; index<10; index++)
    //    cout << "data: " << data[index] << endl;

    //Cleanup
    delete data;

    //Disable pass to next block
    *flag = 0;
    return SendOut(none);//none is a special variable for sinks
}

void doNothing()
{}

//MAIN
int main()
{
    // Create instances of blocks
    Worker block1(Source, 0, 1, doNothing, doNothing, "SRC");
    Worker block2(Sink, 1, 0, doNothing, doNothing, "SINK");

    // Connect blocks together
    connect(block1, 0, block2, 0);

    //// Create graph and add blocks ////
    Graph TestGraph("2 Block Test Graph");
    TestGraph.Blocks = {&block1,&block2};

    // Label Sources and Sinks (Strings from block thread names)
    TestGraph.Sources = {"SRC"};
    TestGraph.Sinks = {"SINK"};

    //// Run Graph ////
    cout<<"MAIN>> Type: 'top -H -p `pidof RX`' in a terminal to view thread utilization\n";
    double SimDuration = 10;// set SimDuration<=0 for continuous run
    cout<<"Starting up (purposely delayed)\n";
    TestGraph.run(SimDuration);// Turn blocks on
    cout<<"Test complete\n";

    return 0;
}
