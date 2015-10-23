//
// File: GenCRC.cpp
//
// MATLAB Coder version            : 2.7
// C/C++ source code generated on  : 22-Oct-2015 19:12:22
//

// Include Files
#include "rt_nonfinite.h"
#include "CheckCRC.h"
#include "GenCRC.h"
#include <stdio.h>

// Custom Source Code
#include "flowMP.h"

// Type Definitions
#ifndef struct_comm_CRCGenerator_0
#define struct_comm_CRCGenerator_0

struct comm_CRCGenerator_0
{
  boolean_T S0_isInitialized;
  boolean_T S1_isReleased;
  unsigned int P0_CRCTable[256];
};

#endif                                 //struct_comm_CRCGenerator_0

typedef struct {
  boolean_T matlabCodegenIsDeleted;
  boolean_T isInitialized;
  boolean_T isReleased;
  comm_CRCGenerator_0 cSFunObject;
} commcodegen_CRCGenerator;

// Variable Definitions
static commcodegen_CRCGenerator crcGen;
static boolean_T crcGen_not_empty;

// Function Definitions

//
// Arguments    : boolean_T output[18]
// Return Type  : void
//
void GenCRC(boolean_T output[18])
{
  commcodegen_CRCGenerator *obj;
  int i;
  static const signed char iv0[256] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };

  comm_CRCGenerator_0 *b_obj;
  unsigned int shReg;
  int bitNumber;
  unsigned int input;
  unsigned int bitMask;
  int j;
  static const boolean_T varargin_1[12] = { true, false, true, true, false, true,
    false, true, true, true, false, true };

  if (!crcGen_not_empty) {
    //  Encode the message words using a CRC generator
    obj = &crcGen;
    crcGen.isInitialized = false;
    crcGen.isReleased = false;

    // System object Constructor function: comm.CRCGenerator
    obj->cSFunObject.S0_isInitialized = false;
    obj->cSFunObject.S1_isReleased = false;
    for (i = 0; i < 256; i++) {
      obj->cSFunObject.P0_CRCTable[i] = (unsigned int)iv0[i];
    }

    crcGen_not_empty = true;
  }

  if (!crcGen.isInitialized) {
    crcGen.isInitialized = true;
  }

  b_obj = &crcGen.cSFunObject;

  // System object Outputs function: comm.CRCGenerator
  // Initialize for current sub-frame
  shReg = 0U;
  bitNumber = 0;
  for (i = 0; i < 3; i++) {
    input = 0U;
    bitMask = 4U;
    for (j = 0; j < 3; j++) {
      if (bitNumber < 6) {
        output[bitNumber] = varargin_1[bitNumber];

        // Build input data chunk
        input |= bitMask * varargin_1[bitNumber];
        bitMask >>= 1U;
        bitNumber++;
      }
    }

    // Retrieve value from CRC table and update register
    shReg = b_obj->P0_CRCTable[shReg & 7U] ^ (shReg << 3U | input);
  }

  // Process the leftover bits
  // Apply final XOR
  // Output the register
  output[6] = ((shReg >> 2U & 1U) != 0U);
  output[7] = ((shReg >> 1U & 1U) != 0U);
  output[8] = ((shReg & 1U) != 0U);

  // Initialize for current sub-frame
  shReg = 0U;
  bitNumber = 0;
  for (i = 0; i < 3; i++) {
    input = 0U;
    bitMask = 4U;
    for (j = 0; j < 3; j++) {
      if (bitNumber < 6) {
        output[9 + bitNumber] = varargin_1[bitNumber + 6];

        // Build input data chunk
        input |= bitMask * varargin_1[bitNumber + 6];
        bitMask >>= 1U;
        bitNumber++;
      }
    }

    // Retrieve value from CRC table and update register
    shReg = b_obj->P0_CRCTable[shReg & 7U] ^ (shReg << 3U | input);
  }

  // Process the leftover bits
  // Apply final XOR
  // Output the register
  output[15] = ((shReg >> 2U & 1U) != 0U);
  output[16] = ((shReg >> 1U & 1U) != 0U);
  output[17] = ((shReg & 1U) != 0U);
}

//
// Arguments    : void
// Return Type  : void
//
void crcGen_not_empty_init()
{
  crcGen_not_empty = false;
}

//
// File trailer for GenCRC.cpp
//
// [EOF]
//
