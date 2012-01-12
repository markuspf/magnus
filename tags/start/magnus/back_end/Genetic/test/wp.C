/*
 *   $Id: wp.C 4 2005-01-07 00:28:20Z daly $
 */

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
#include "GAWP.h"

int main ( )
{
  Chars errMsg;
  GHNConfig c;
  //cout << c << endl;

  ifstream in("../wp.in");
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

  GAWP gawp(G,c);
  gawp.isTrivial(w, cout);
}
