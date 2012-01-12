// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Global operators for Interprocess Communication (IPC).
//
// Principal Author: Dmitry Bormotov
//
// Status: Under development
//
// Special Notes:
//
//   This operators are for special cases, i.e. they are seldom useable. 
//

#ifndef _EXTENDED_IPC_H_
#define _EXTENDED_IPC_H_


#include "global.h"
#include "Integer.h"
#include "Rational.h"
using std::ostream;
using std::istream;

// ------------------------- Output operators ------------------------------ //


inline std::ostream& operator < ( std::ostream& ostr, const Integer& n )
{
  ostr << n << ' ';
  return ostr;
}

inline std::ostream& operator < ( std::ostream& ostr, const Rational& r )
{
  ostr << r << ' ';
  return ostr;
}


// ------------------------- Input operators ------------------------------- //


inline std::istream& operator > ( std::istream& istr, Integer& n )
{
  istr >> n;
  return istr;
}

inline std::istream& operator > ( std::istream& istr, Rational& r )
{
  istr >> r;
  return istr;
}


#endif

