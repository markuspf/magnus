// Copyright (C) 1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Definition and implementation of class Pair
//
// Principal Author: Dmitry Bormotov
//
// Status: Under development
//
// Revision History:


#ifndef _PAIR_H_
#define _PAIR_H_


#include <iostream.h>
#include "IPC.h"

struct Pair
{
  Pair(int x, int y) : a(x), b(y) { }

  Pair( ) { }

  bool operator == (const Pair& p) const 
    { return a == p.a && b == p.b; }

  friend ostream& operator << ( ostream& out, const Pair& p ) {
    out << " (" << p.a << "," << p.b << ")";
  }

  friend ostream& operator < ( ostream& ostr, const Pair& ) { return ostr; }

  friend istream& operator > ( istream& istr, Pair& ) { return istr; }

  int hash() const { return a;}
  
  int a,b;
};

#endif
