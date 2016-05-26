// Auto-Generated Header
// MATLAB Datatype support functions (May not always be included with generated code, if not they are not required here)
//#include "RX_emxutil.h"

/////////////////////////////////////////////////
OUTPUTS DumpSignalBLK(INPUTS input, int *flag)
{
    // Get input(s)
    const creal_T *dataSig = (const creal_T*) input[0];

    // No outputs

    // MATLAB Function Call
    DumpSignal(dataSig);

    // Cleanup
    delete dataSig;

    // Send Nothing
    return SendOut(none);

}

/////////////////////////////////////////////////
OUTPUTS GenSignalBLK(INPUTS input, int *flag)
{
    // No Inputs

    // Setup output(s)
    creal_T *output = new creal_T[64];

    // MATLAB Function Call
    GenSignal(output);

    // Send Out
    *flag = 1;
    return SendOut(output);

}
//[EOF]
