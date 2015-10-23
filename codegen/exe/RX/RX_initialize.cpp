//
// File: RX_initialize.cpp
//
// MATLAB Coder version            : 2.7
// C/C++ source code generated on  : 22-Oct-2015 19:12:22
//

// Include Files
#include "rt_nonfinite.h"
#include "CheckCRC.h"
#include "GenCRC.h"
#include "RX_initialize.h"
#include <stdio.h>

// Custom Source Code
#include "flowMP.h"

// Function Definitions

//
// Arguments    : void
// Return Type  : void
//
void RX_initialize()
{
  rt_InitInfAndNaN(8U);
  crcDetect_not_empty_init();
  crcGen_not_empty_init();
}

//
// File trailer for RX_initialize.cpp
//
// [EOF]
//
