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

#include "Afx.h"
#include "error.h"
#include "IPC.h"
#include "Trichotomy.h"

//#define NULL 0

#define SAFETY 2

#define TRUE  1
#define FALSE 0

typedef int Bool;

#define INFINITE 0


//------ global function prototypes, variables, inline functions ------------//

inline int min(int i, int j) { return ((i<j) ? i : j); }
inline int max(int i, int j) { return ((i>j) ? i : j); }
inline int abs(int i) { return ((i < 0) ? -i : i); }
inline int sign(int i) { return (i == 0) ? 0 : ( (i > 0) ? 1 : -1 ); }
#endif
