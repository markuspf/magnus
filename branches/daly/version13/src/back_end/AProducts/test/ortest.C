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


#include "OneRelatorGroup.h"
#include "FreeGroup.h"

int main ( )
{
  FPGroup G;
  cout << "Enter an OR group: ";
  cin >> G;
  cout << endl;
  OneRelatorGroup O(G);

  Word r = O.relator();
  int rLen = r.length();
  int ng = O.numberOfGenerators();
  int i = 0;
  int oldLen = 0;

  FreeGroup F(ng);

  while( true ) {

    Word w  = F.getN_thElement(++i);
    if( w.length() >= rLen ) {
      cout << "There's no such word." << endl;
      break;
    }
    
    if( O.wordProblem(w) == yes ) {
      F.printWord(cout, w);
      cout << endl;
      break;
    }
  }
}

