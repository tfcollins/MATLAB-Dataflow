#include "flowMP.h"
#include "utils.h"
#include "graph.h"
//#ifdef _WIN32
//    #include <Windows.h>
//#endif

#define PRINT 0

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
        data[index] = 1.0*index+1;

    //Enable pass to next block
    *flag = 1;
    return SendOut(data);
}

OUTPUTS DoNothingBLK(INPUTS input, int *flag)
{
  // Get input
  std::shared_ptr<double> data ((double*) input[0]);

  // Setup output
  double *data2 = new double[10];

  // Process
  std::memcpy(data2,data.get(),sizeof(double)*10);

  *flag = 1;
  return SendOut(data2);
}


/////////////////////////////////////////////////
OUTPUTS Sink(INPUTS input, int *flag)
{
    // Get input
    std::shared_ptr<double> ddata ((double*) input[0]);

    // Process
    if (PRINT)
    {
      for(int index=0; index<10; index++)
        cout << "data: " << *(ddata.get()+index) << endl;
    }

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
    Worker block2(DoNothingBLK, 1, 1, doNothing, doNothing, "DN");
    Worker block3(Sink, 1, 0, doNothing, doNothing, "SINK");

    // Connect blocks together
    connect(block1, 0, block2, 0);
    connect(block2, 0, block3, 0);

    //// Create graph and add blocks ////
    Graph TestGraph("2 Block Test Graph");
    TestGraph.Blocks = {&block1,&block2,&block3};

    // Enable Benchmarking
    block2.m_BenchMarkingCount = 1000;

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
