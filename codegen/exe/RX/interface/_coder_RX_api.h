/* 
 * File: _coder_RX_api.h 
 *  
 * MATLAB Coder version            : 2.7 
 * C/C++ source code generated on  : 22-Oct-2015 19:12:22 
 */

#ifndef ___CODER_RX_API_H__
#define ___CODER_RX_API_H__
/* Include Files */ 
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Function Declarations */ 
extern void RX_initialize(emlrtContext *aContext);
extern void RX_terminate(void);
extern void RX_atexit(void);
extern void GenCRC_api(const mxArray *plhs[1]);
extern void GenCRC(boolean_T output[18]);
extern void CheckCRC_api(const mxArray *prhs[1]);
extern void CheckCRC(boolean_T codeword[18]);
extern void RX_xil_terminate(void);

#endif
/* 
 * File trailer for _coder_RX_api.h 
 *  
 * [EOF] 
 */
