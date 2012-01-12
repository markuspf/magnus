// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test of class GeneralWhitehead.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include "GeneralWhitehead.h"

int main ( )
{
  Chars errMsg;
  FreeGroup F;
  cout << "Enter a free group: ";
  errMsg = cin >> F;
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl << "F = " << F << endl;

  cout << "Enter a word in F: ";
  Word w = F.readWord(cin, errMsg);
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  
  cout << endl;
  F.printWord(cout, w);
  cout << endl;

  int wLen = w.length();
  Word a,b;
  for( int i = 1; i < wLen; ++i )
    if( F.isCommutator(w.initialSegment(i), a, b) ) {
      cout << "Found a commutator of length " << i << ": [";
      F.printWord(cout, a);
      cout << ",";
      F.printWord(cout, b);
      cout << "]" << endl;
      break;
    }
  if( i >= wLen )
    cout << "There is no commutator" << endl;
}

