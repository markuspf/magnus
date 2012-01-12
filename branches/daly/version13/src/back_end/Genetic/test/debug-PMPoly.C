// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


//#include "Int2.h"
#include "global.h"
#include "Word.h"
#include "Map.h"
#include "PMPolynomial.h"


int main ( )
{
  Word w;
  Map M;
  Polynomial<Int2> p("x1 x2 x3,");
  UniformRandom r;
  PMPolyConfig PMPC(r,10,5,3,15);
  PMPolynomial pp(PMPC,p);
  cout << "Result: " << ((PMPolynomial*)(pp.mutate()))->getPolynomial() 
       << endl;
}


