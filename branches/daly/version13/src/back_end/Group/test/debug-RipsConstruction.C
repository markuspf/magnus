// Copyright (C) 2000 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test of class RipsConstruction.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//

#include "Matrix.h"
#include "Polynomial.h"
#include "Map.h"
#include "RipsConstruction.h"

int main ( )
{
  Matrix< Polynomial<Integer> > I(4);
  Polynomial<Integer> p("1-x1,");
  Map M;

  Chars errMsg;
  FPGroup G;
  cout << "Enter a FP group: ";
  errMsg = cin >> G;
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl << "G = " << G << endl;

  FPGroup R = RipsConstruction(G).getRipsConstruction();
  cout << "Num of rels: " << R.getRelators().cardinality() << endl;

  cout << "Rips Construction: " << endl << endl 
       << R << endl;
}

