
// MATLAB Datatype support functions (May not always be included with generated code, if not they are not required here)
//#include "RX_emxutil.h"

// Defines
#define NUMSAMPLES 18

//***Block Definitions***//

/////////////////////////////////////////////////
OUTPUTS CRCSource(INPUTS input, int *flag)
{
    usleep(1000);// Slow this down so we don't max out the core
    
    // Setup output
    boolean_T *bits = new boolean_T[NUMSAMPLES];
    
    // Process
    GenCRC(bits);
            
    //Enable pass to next block
    *flag = 1;
    return SendOut(bits);
}
/////////////////////////////////////////////////
OUTPUTS CRCSink(INPUTS input, int *flag)
{
    // Get input
    boolean_T *bits = (boolean_T*) input[0];
    
    // Process
    CheckCRC(bits);
            
    //Cleanup
    delete bits;

    //Disable pass to next block
    //*flag = 0;
    return SendOut(none);//none is a special variable for sinks
}
// [EOF]
