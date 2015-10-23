//
// File: fprintf.cpp
//
// MATLAB Coder version            : 2.7
// C/C++ source code generated on  : 22-Oct-2015 19:12:22
//

// Include Files
#include "rt_nonfinite.h"
#include "CheckCRC.h"
#include "GenCRC.h"
#include "fprintf.h"
#include "fileManager.h"
#include <stdio.h>

// Custom Source Code
#include "flowMP.h"

// Function Declarations
static double c_fprintf(long varargin_1, long varargin_2);

// Function Definitions

//
// Arguments    : long varargin_1
//                long varargin_2
// Return Type  : double
//
static double c_fprintf(long varargin_1, long varargin_2)
{
  int nbytesint;
  boolean_T autoflush;
  FILE * filestar;
  static const char cfmt[29] = { 'C', 'R', 'C', ' ', 'c', 'h', 'e', 'c', 'k',
    ' ', 'r', 'e', 's', 'u', 'l', 't', ':', ' ', '[', '%', 'l', 'd', ' ', '%',
    'l', 'd', ']', '\x0a', '\x00' };

  nbytesint = 0;
  fileManager(&filestar, &autoflush);
  if (filestar == NULL) {
  } else {
    nbytesint = fprintf(filestar, cfmt, varargin_1, varargin_2);
    if (autoflush) {
      fflush(filestar);
    }
  }

  return nbytesint;
}

//
// Arguments    : long formatSpec
//                long varargin_1
// Return Type  : void
//
void b_fprintf(long formatSpec, long varargin_1)
{
  c_fprintf(formatSpec, varargin_1);
}

//
// File trailer for fprintf.cpp
//
// [EOF]
//
