/*
 *   $Id: Range.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents:     
//               
//
// Principal Author: Dmitry Pechkin
//
// Status: under trial.
//
// Revision History:
//
// TO DO:
//
//

#include "Range.h"
#include "global.h"
#include "CONDITION.h"

void operator < ( ostream& ostr, const Range& r )
{
  ostr < r.low;
  ostr < ' ';
  ostr < r.high;
}

void operator > ( istream& istr, Range& r )
{
  istr > r.low;
  istr > r.high;
}

ostream& operator << ( ostream& ostr, const Range& r )
{
  ostr << r.low << ':' << r.high;
  return ostr;
}

istream& operator >> ( istream& istr, Range& r )
{
  istr >> r.low;

  char ch;
  istr >> ch;
  CONDITION( WARNING, ch == ':' );

  istr >> r.high;

  return istr;
}
