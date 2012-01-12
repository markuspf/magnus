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

#include "RSWord.h"
#include "FreeGroup.h"
#include "Polynomial.h"

void main ( ) 
{
  //@db to shut up the bad linker
  Polynomial<Integer> P;
  Map M;

  Chars errMsg;
  FreeGroup F;
  cout << "Enter a free group: ";
  errMsg = cin >> F;
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;

  cout << "Enter a word, such that a letter 't' occurs in it and the exponent"
    " sum of 't' is 0: ";
  Word w;
  w = F.readWord(cin,errMsg);
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;

  cout << "Your word: ";
  F.printWord(cout,w);
  cout << endl << endl;

  RSWord RSW(F,w);
  cout << "Rewritten word: ";
  cout << RSW << endl;
}
