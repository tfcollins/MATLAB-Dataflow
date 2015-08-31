#include <time.h>
#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "CFOCorrect.h"
#include "CFOCorrect_initialize.h"
#include "CFOCorrect_terminate.h"
//#include "BM.h"

#define BILLION  1000000000L;

using namespace std;

int main()
{
    
    // Create inputs
    creal_T *data = new creal_T[8000];
    for (int i=0;i<8000;i++)
    {
        data[i].re = (double) i;
        data[i].im = (double) i-1;
    }
    boolean_T *foundPackets = new boolean_T[100];
    double *freqEstimates = new double[100];
    for (int i=0;i<100;i++)
    {
        if (i%1000==0)
            foundPackets[i] = true;
        else
            foundPackets[i] = false;
        
        freqEstimates[i] = (double) i;
    }
    
//     cout<<"Starting Benchmark\n";
//     CFOCorrect_initialize;
//     
//     // reset the clock
//     timespec tS;
//     tS.tv_sec = 0;
//     tS.tv_nsec = 0;
//     clock_settime(CLOCK_PROCESS_CPUTIME_ID, &tS);
// 
//     // Process
//     for (int k=0; k<1000; k++)
//         CFOCorrect(data,foundPackets,freqEstimates);
//     
//     clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tS);
//     //cout << "Time taken is: " << tS.tv_sec << " " << tS.tv_nsec << endl;
//     cout << "Time taken is: " << tS.tv_nsec << endl;
//     double seconds = ((double) tS.tv_nsec)/1000000000;
//     double throughputMSPS = 8/seconds;
//     cout<<"Throughput: "<<throughputMSPS<<" (MS/s)\n";
//     
//     CFOCorrect_terminate;
    

    ////////////////////////////////////////////////////////
    CFOCorrect_init();
    struct timespec start, stop;
    double accum;
    
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
    }
    
    // Process
    double runs = 1000;
    for (int k=0; k<runs; k++)
    {
        CFOCorrect(data,foundPackets,freqEstimates);
        //cout<<"Test: "<<k<<endl;
    }
        //usleep(1000000);
    
    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
    }
    
    accum = ( stop.tv_sec - start.tv_sec )
    + ( stop.tv_nsec - start.tv_nsec )
    / BILLION;
    
    double seconds = ((double)( stop.tv_sec - start.tv_sec ))+((double)( stop.tv_nsec - start.tv_nsec ))/1000000000;
    cout<<"Duration: "<<seconds<<endl;
    cout<<"Data: "<<8000*runs<<endl;
    
    double throughputMSPS2 = ((double) 8000*runs )/ seconds;
    cout<<"Throughput: "<<throughputMSPS2/1000000<<" (MS/s)\n";
    CFOCorrect_terminate;

    ////////////////////////////////////////////////////////
    
    return 0;
    
}