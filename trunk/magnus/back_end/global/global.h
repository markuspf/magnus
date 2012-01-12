// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Global declarations to be included by all other header files.
//
// Principal Author: Roger Needham
//
// Status: Useable.
//
// Further implemenation steps:
//
// Revision History:
//
// * 5/27/95 Roger removed the inclusion of config.h.
//           See general/MagnusHome.h
//
// * 1/96 Dmitry B. added the inclusion of IPC.h
//

#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#include <iostream>
#include <fstream>
//@njz
#include <sstream>
//
//@rn #include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//@njz
#include "config.h"
//

#include "error.h"
#include "IPC.h"
#include "Trichotomy.h"

//#define NULL 0

#define SAFETY 2

#define TRUE  1
#define FALSE 0

typedef int Bool;


#define INFINITE 0

//@njz

/* Test for GCC >= 3.2.0 */
//#if GCC_VERSION >= 30200

#ifdef __GNUC_PATCHLEVEL__
#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
#else
#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100)
#endif


#include <limits.h>
#if !defined(MAXINT) && defined(INT_MAX)
#define MAXINT INT_MAX
#endif
//

//------ global function prototypes, variables, inline functions ------------//

#include <algorithm>
using std::max;
using std::min;

/*
template <class T> T max(T a, T b) {
   if(a > b)
      return a;
   else
      return b;
}

template <class T> T min(T a, T b) {
   if(a > b)
      return b;
   else
      return a;
}
*/
template <class T> int sign(T n) {
   if(n > T(0))
      return 1;
   else
      if(n < T(0))
         return -1;
      else
         return 0;
}

template <class T> T abs(T n) {
   if(n > T(0))
      return n;
   else
      return -n;
}
#endif
