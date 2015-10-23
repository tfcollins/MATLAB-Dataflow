//
// File: fileManager.cpp
//
// MATLAB Coder version            : 2.7
// C/C++ source code generated on  : 22-Oct-2015 19:12:22
//

// Include Files
#include "rt_nonfinite.h"
#include "CheckCRC.h"
#include "GenCRC.h"
#include "fileManager.h"
#include <stdio.h>

// Custom Source Code
#include "flowMP.h"

// Function Definitions

//
// Arguments    : FILE * *f
//                boolean_T *a
// Return Type  : void
//
void fileManager(FILE * *f, boolean_T *a)
{
  *f = stdout;
  *a = true;
}

//
// File trailer for fileManager.cpp
//
// [EOF]
//
