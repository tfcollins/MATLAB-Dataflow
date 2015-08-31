#include "flowMP.h"
#include "utils.h"
//#ifdef _WIN32
//    #include <Windows.h>
//#endif

using namespace std;

// Function to do work
OUTPUTS Block1(INPUTS input, int *flag)
{
    usleep(100);
    OUTPUTS output(2);
        
    double *array1 = new double[4];
    double *array2 = new double[4];
    
    for(int x=0;x<4;x++)
    {
        array1[x] = x;
    }
    
    memcpy(array2,array1,4*sizeof(double));
    
    output[0] = (void*)array1;
    output[1] = (void*)array2;
    
    *flag = 1;//Enable pass to next block
    return output;
}

// Function to do work
OUTPUTS Block2(INPUTS input, int *flag)
{   
    
    std::vector<void*> output(1);
    
    double *input0 = (double*) input[0];
    
    
    for(int x=0;x<4;x++)
        cout<<input0[x]*2<<endl;
    
    // Cleanup input
    delete input0;
    //Enable pass to next block
    *flag = 1;
    return output;
}
// Function to do work
OUTPUTS Block3(INPUTS input, int *flag)
{   
    cout<<"CALLED\n";
    std::vector<void*> output(1);
    
    double *input0 = (double*) input[0];
    
    for(int x=0;x<4;x++)
        cout<<input0[x]<<endl;
    
    // Cleanup input
    delete input0;
    //Enable pass to next block
    *flag = 1;
    return output;
}

void doNothing()
{}

//MAIN
int main()
{
    // Create instances of blocks
    int numPorts = 1;
    Worker block1(Block1, 1, 2, doNothing, doNothing);
    Worker block2(Block2, 1, 1, doNothing, doNothing);
    Worker block3(Block3, 1, 1, doNothing, doNothing);
    
    // Connect blocks together
    connect(block1, 0, block2, 0);
    connect(block1, 1, block3, 0);
    
    // Turn blocks on
    block1.run_source();
    block2.run_sink();
    block3.run_sink();
    
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