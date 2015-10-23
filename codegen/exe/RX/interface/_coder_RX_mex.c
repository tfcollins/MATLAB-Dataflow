/*
 * _coder_RX_mex.c
 *
 * Code generation for function 'GenCRC'
 *
 */

/* Include files */
#include "mex.h"
#include "_coder_RX_api.h"

/* Function Declarations */
static void CheckCRC_mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);
static void GenCRC_mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]);

/* Variable Definitions */
emlrtContext emlrtContextGlobal = { true, false, EMLRT_VERSION_INFO, NULL, "RX", NULL, false, {2045744189U,2170104910U,2743257031U,4284093946U}, NULL };
void *emlrtRootTLSGlobal = NULL;
emlrtEntryPoint emlrtEntryPoints[2] = {
  { "CheckCRC", CheckCRC_mexFunction },
  { "GenCRC", GenCRC_mexFunction },
};

/* Function Definitions */
static void GenCRC_mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  const mxArray *outputs[1];
  int n = 0;
  int nOutputs = (nlhs < 1 ? 1 : nlhs);
  emlrtStack st = { NULL, NULL, NULL };
  /* Module initialization. */
  RX_initialize(&emlrtContextGlobal);
  st.tls = emlrtRootTLSGlobal;
  /* Check for proper number of arguments. */
  if (nrhs != 0) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, mxINT32_CLASS, 0, mxCHAR_CLASS, 6, "GenCRC");
  } else if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, mxCHAR_CLASS, 6, "GenCRC");
  }
  /* Call the function. */
  GenCRC_api(outputs);
  /* Copy over outputs to the caller. */
  for (n = 0; n < nOutputs; ++n) {
    plhs[n] = emlrtReturnArrayR2009a(outputs[n]);
  }
  /* Module finalization. */
  RX_terminate();
}
static void CheckCRC_mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  const mxArray *inputs[1];
  int n = 0;
  int nInputs = nrhs;
  emlrtStack st = { NULL, NULL, NULL };
  /* Module initialization. */
  RX_initialize(&emlrtContextGlobal);
  st.tls = emlrtRootTLSGlobal;
  /* Check for proper number of arguments. */
  if (nrhs != 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, mxINT32_CLASS, 1, mxCHAR_CLASS, 8, "CheckCRC");
  } else if (nlhs > 0) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, mxCHAR_CLASS, 8, "CheckCRC");
  }
  /* Temporary copy for mex inputs. */
  for (n = 0; n < nInputs; ++n) {
    inputs[n] = prhs[n];
  }
  /* Call the function. */
  CheckCRC_api(inputs);
  /* Module finalization. */
  RX_terminate();
}

void RX_atexit_wrapper(void)
{
   RX_atexit();
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  emlrtMexFunction method;
  method = emlrtGetMethod(nrhs, prhs, emlrtEntryPoints, 2);
  /* Initialize the memory manager. */
  mexAtExit(RX_atexit_wrapper);
  /* Dispatch the entry-point. */
  method(nlhs, plhs, nrhs-1, prhs+1);
}
/* End of code generation (_coder_RX_mex.c) */
