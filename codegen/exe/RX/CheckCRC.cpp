//
// File: CheckCRC.cpp
//
// MATLAB Coder version            : 2.7
// C/C++ source code generated on  : 22-Oct-2015 19:12:22
//

// Include Files
#include "rt_nonfinite.h"
#include "CheckCRC.h"
#include "GenCRC.h"
#include "fprintf.h"
#include <stdio.h>

// Custom Source Code
#include "flowMP.h"

// Type Definitions
#ifndef struct_comm_CRCDetector_1
#define struct_comm_CRCDetector_1

struct comm_CRCDetector_1
{
  boolean_T S0_isInitialized;
  boolean_T S1_isReleased;
  unsigned int P0_CRCTable[256];
};

#endif                                 //struct_comm_CRCDetector_1

typedef struct {
  boolean_T matlabCodegenIsDeleted;
  boolean_T isInitialized;
  boolean_T isReleased;
  comm_CRCDetector_1 cSFunObject;
} commcodegen_CRCDetector;

// Variable Definitions
static commcodegen_CRCDetector crcDetect;
static boolean_T crcDetect_not_empty;

// Function Definitions

//
// Asserts
// Arguments    : const boolean_T codeword[18]
// Return Type  : void
//
void CheckCRC(const boolean_T codeword[18])
{
  commcodegen_CRCDetector *obj;
  int i;
  static const signed char iv1[256] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0,
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

  comm_CRCDetector_1 *b_obj;
  unsigned int shReg;
  int bitNumber;
  unsigned int inpChksum;
  unsigned int bitMask;
  int j;
  boolean_T varargout_2_idx_0;
  boolean_T b_inpChksum;
  if (!crcDetect_not_empty) {
    obj = &crcDetect;
    crcDetect.isInitialized = false;
    crcDetect.isReleased = false;

    // System object Constructor function: comm.CRCDetector
    obj->cSFunObject.S0_isInitialized = false;
    obj->cSFunObject.S1_isReleased = false;
    for (i = 0; i < 256; i++) {
      obj->cSFunObject.P0_CRCTable[i] = (unsigned int)iv1[i];
    }

    crcDetect_not_empty = true;
  }

  //  Decode messages with and without errors using a CRC decoder
  if (!crcDetect.isInitialized) {
    crcDetect.isInitialized = true;
  }

  b_obj = &crcDetect.cSFunObject;

  // System object Outputs function: comm.CRCDetector
  // Initialize for current sub-frame
  shReg = 0U;
  bitNumber = 0;
  for (i = 0; i < 3; i++) {
    inpChksum = 0U;
    bitMask = 4U;
    for (j = 0; j < 3; j++) {
      if (bitNumber < 6) {
        // Build input data chunk
        inpChksum |= bitMask * codeword[bitNumber];
        bitMask >>= 1U;
        bitNumber++;
      }
    }

    // Retrieve value from CRC table and update register
    shReg = b_obj->P0_CRCTable[shReg & 7U] ^ (shReg << 3U | inpChksum);
  }

  // Process the leftover bits
  // Apply final XOR
  // Output flag to indicate if error occurred (0 => no error)
  inpChksum = 0U;
  for (i = 0; i < 3; i++) {
    inpChksum <<= 1U;
    inpChksum ^= codeword[i + 6];
  }

  inpChksum = (inpChksum ^ shReg) << 29U;
  if (inpChksum == 0U) {
    varargout_2_idx_0 = false;
  } else {
    varargout_2_idx_0 = true;
  }

  // Initialize for current sub-frame
  shReg = 0U;
  bitNumber = 0;
  for (i = 0; i < 3; i++) {
    inpChksum = 0U;
    bitMask = 4U;
    for (j = 0; j < 3; j++) {
      if (bitNumber < 6) {
        // Build input data chunk
        inpChksum |= bitMask * codeword[bitNumber + 9];
        bitMask >>= 1U;
        bitNumber++;
      }
    }

    // Retrieve value from CRC table and update register
    shReg = b_obj->P0_CRCTable[shReg & 7U] ^ (shReg << 3U | inpChksum);
  }

  // Process the leftover bits
  // Apply final XOR
  // Output flag to indicate if error occurred (0 => no error)
  inpChksum = 0U;
  for (i = 0; i < 3; i++) {
    inpChksum <<= 1U;
    inpChksum ^= codeword[15 + i];
  }

  inpChksum = (inpChksum ^ shReg) << 29U;
  if (inpChksum == 0U) {
    b_inpChksum = false;
  } else {
    b_inpChksum = true;
  }

  b_fprintf((long)varargout_2_idx_0, (long)b_inpChksum);
}

//
// Arguments    : void
// Return Type  : void
//
void crcDetect_not_empty_init()
{
  crcDetect_not_empty = false;
}

//
// File trailer for CheckCRC.cpp
//
// [EOF]
//
