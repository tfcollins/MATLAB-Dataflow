/*
 * File: _coder_RX_api.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 22-Oct-2015 19:12:22
 */

/* Include Files */
#include "_coder_RX_api.h"

/* Function Declarations */
static const mxArray *emlrt_marshallOut(const boolean_T u[18]);
static boolean_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *codeword, const char_T *identifier))[18];
static boolean_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[18];
static boolean_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[18];

/* Function Definitions */

/*
 * Arguments    : emlrtContext *aContext
 * Return Type  : void
 */
void RX_initialize(emlrtContext *aContext)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, aContext, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void RX_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void RX_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  RX_xil_terminate();
}

/*
 * Arguments    : const mxArray *plhs[1]
 * Return Type  : void
 */
void GenCRC_api(const mxArray *plhs[1])
{
  boolean_T (*output)[18];
  output = (boolean_T (*)[18])mxMalloc(sizeof(boolean_T [18]));

  /* Invoke the target function */
  GenCRC(*output);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(*output);
}

/*
 * Arguments    : const boolean_T u[18]
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const boolean_T u[18])
{
  const mxArray *y;
  static const int32_T iv0[1] = { 0 };

  const mxArray *m0;
  static const int32_T iv1[1] = { 18 };

  y = NULL;
  m0 = emlrtCreateLogicalArray(1, iv0);
  mxSetData((mxArray *)m0, (void *)u);
  emlrtSetDimensions((mxArray *)m0, *(int32_T (*)[1])&iv1[0], 1);
  emlrtAssign(&y, m0);
  return y;
}

/*
 * Arguments    : const mxArray *prhs[1]
 * Return Type  : void
 */
void CheckCRC_api(const mxArray *prhs[1])
{
  boolean_T (*codeword)[18];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, false, -1);

  /* Marshall function inputs */
  codeword = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "codeword");

  /* Invoke the target function */
  CheckCRC(*codeword);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *codeword
 *                const char_T *identifier
 * Return Type  : boolean_T (*)[18]
 */
static boolean_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray
  *codeword, const char_T *identifier))[18]
{
  boolean_T (*y)[18];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = b_emlrt_marshallIn(sp, emlrtAlias(codeword), &thisId);
  emlrtDestroyArray(&codeword);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : boolean_T (*)[18]
 */
  static boolean_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[18]
{
  boolean_T (*y)[18];
  y = c_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : boolean_T (*)[18]
 */
static boolean_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[18]
{
  boolean_T (*ret)[18];
  int32_T iv2[1];
  iv2[0] = 18;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "logical", false, 1U, iv2);
  ret = (boolean_T (*)[18])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
/*
 * File trailer for _coder_RX_api.c
 *
 * [EOF]
 */
