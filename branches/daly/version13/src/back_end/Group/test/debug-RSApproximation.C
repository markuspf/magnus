// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test for RSWord
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//

#include "RSApproximation.h"
#include "FPGroup.h"
#include "Map.h"
#include "Polynomial.h"

void main ( ) 
{
  //@db to shut up the bad linker
  Polynomial<Integer> P;
  Map M;

  Chars errMsg;
  FPGroup G;
  cout << "Enter fp group, the first generator of which is 't' and such"
    " that the exponent sum of 't' in each relator is zero: ";
  errMsg = cin >> G;
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;

  cout << "Your group: ";
  cout << G;
  cout << endl << endl;

  int l;
  cout << "Enter parameter l: ";
  cin >> l;
  cout << "l = " << l;
  cout << endl << endl;

  RSApproximation RSA(G);
  SetOf<RSWord> S = RSA.getApproximation(l);
  
  cout << "The list of relators: " << endl << endl;

  SetIterator<RSWord> I(S);
  for( ; !I.done(); I.next() ) 
    cout << I.value() << endl;
}
