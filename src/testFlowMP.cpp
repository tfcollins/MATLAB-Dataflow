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
    usleep(1000);// Slow this down so we don't max out the core

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
    // Process
    for(int index=0; index<10; index++)
        cout << "data: " << data[index] << endl;

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

    // Turn blocks on
    block1.run_source();
    block2.run_sink();

    // Setup Timers
    clock_t init, final, mid;
    init=clock();

    // Wait for Blocks to start
    for(int k=0;k<1000;k++)
        usleep(100000);
    cout<<"Blocks should be started now\n";

    // Monitor Queues
    /*
    for (int j=0; j<10000; j++)
    {
        usleep(10000);

        int b1Qout = *(block1.m_OutputQueueSize);
        int b2Qin = *(block2.m_InputQueueSize);

        if (b1Qout>10)
            cout<<"block1 Output Queue Large, Size: "<<b1Qout<<endl;
        if (b2Qin>10)
            cout<<"block2 Input Queue Large, Size: "<<b2Qin<<endl;

    }
    */
    cout<<"Main thread sleeping\n";

    for(int j=0;j<100;j++)
        usleep(100);

    cout<<"Main thread done sleeping\n";

    // Stop Timers
    final=clock()-init;
    cout << (double)final / ((double)CLOCKS_PER_SEC) << " Seconds" << endl;

    // Shutdown
    cout<<"------SHUTTING DOWN------\n";
    cout<<"Waiting for thread to quit\n";
    block1.m_StopThread = true;
    block2.m_StopThread = true;
    usleep(1000000);
    block1.m_BlockThread.join();
    usleep(1000000);
    block2.m_BlockThread.join();

    return 0;
}
