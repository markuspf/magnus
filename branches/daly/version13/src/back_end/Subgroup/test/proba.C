// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include "global.h"
#include "Proba.h"
#include "SubgroupGraph.h"
#include "FreeGroup.h"
#include "Word.h"


int main ( )
{
  FreeGroup F(4);
  VectorOf<Generator> v(4);
  for( int i = 0; i < 4; ++i )
    v[i] = Generator(i+1);
  Word w1(v);
  v[0] = 1; v[1] = 2; v[2] = -1; v[3] = -2;
  Word w2(v);
  SetOf<Word> S(w1);
  //S |= w2;

  SubgroupGraph A(4,S);
  A.debugPrint(cout);
  cout << endl;
  
  A.nielsenBasis();
  int dist = A.look()->distanceFromOrigin(1);
  cout << "dist = " << dist << endl;
}
