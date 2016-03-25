
// MATLAB Datatype support functions (May not always be included with generated code, if not they are not required here)
//#include "RX_emxutil.h"

// Defines
#define NUMSAMPLES 18

// Globals
long counter = 1;
long maxCount = 10000000; // Number of block handovers to accomplish
boost::timer::cpu_times times;
boost::timer::cpu_timer timer;
bool start = false;

//***Block Definitions***//

/////////////////////////////////////////////////
OUTPUTS Producer(INPUTS input, int *flag)
{
    // usleep(1);// Slow this down so we don't max out the core
    if (!start)
    {
        std::cout<<"Starting Timer\n";
        start = true;
        timer.start();
    }
    // Setup output
    bool *bits;// = bool[NUMSAMPLES];

    //Enable pass to next block
    *flag = 1;
    return SendOut(bits);
}
/////////////////////////////////////////////////
OUTPUTS Consumer(INPUTS input, int *flag)
{
    // Get input
    bool *bits = (bool*) input[0];

    if (counter==maxCount)
    {
        timer.stop();
        times = timer.elapsed();
        std::cout<<"Stopping Timer\n";
        std::cout << timer.format() << '\n';
        counter++;
    }
    else if (counter<maxCount)
        counter++;

    //Disable pass to next block
    return SendOut(none);//none is a special variable for sinks
}
// [EOF]
