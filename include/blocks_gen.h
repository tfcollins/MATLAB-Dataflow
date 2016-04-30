// MATLAB Datatype support functions (May not always be included with generated code, if not they are not required here)
//#include "RX_emxutil.h"

/////////////////////////////////////////////////
OUTPUTS GenCRCBLK(INPUTS input, int *flag)
{
    // No Inputs

    // Setup output(s)
    boolean_T *output = new boolean_T[18];

    // MATLAB Function Call
    GenCRC(output);

    // Send Out
    *flag = 1;
    return SendOut(output);

}

/////////////////////////////////////////////////
OUTPUTS CheckCRCBLK(INPUTS input, int *flag)
{
    // Get input(s)
    const boolean_T *codeword = (const boolean_T*) input[0];

    // No outputs

    // MATLAB Function Call
    CheckCRC(codeword);

    // Cleanup
    delete codeword;

    // Send Nothing
    return SendOut(none);

}
//[EOF]
