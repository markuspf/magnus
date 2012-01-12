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


#include <values.h>
#include "global.h"
#include "Map.h"
#include "Config.h"
#include "RandomNumbers.h"
#include "Roulette.h"
#include "GeneticGraphWP.h"

int main ( )
{
  Chars errMsg;
  GHNConfig c;
  //cout << c << endl;

  ifstream in("../graphWP.in");
  in >> c;
  cout << c << endl;

  FPGroup G;
  cout << "Enter a finitely presented group: ";
  errMsg = cin >> G;
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;
  
  cout << "Enter a word: ";
  Word w;
  w = G.readWord(cin,errMsg);
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;

  GeneticGraphWP wp(G,c);
  wp.isTrivial(w, &cout);
}
