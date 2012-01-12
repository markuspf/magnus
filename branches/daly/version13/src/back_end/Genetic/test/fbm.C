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
#include "FreeGroup.h"
#include "Config.h"
#include "RandomNumbers.h"
#include "GAMatrixBurau.h"
#include "GAFastBurMat.h"
#include "File.h"


int main ( )
{
  Map M;
  Chars errMsg;
  GAConfig c(50,-1,100,100,GAConfig::STRONG,true); 
  cout << c << endl;
 
  FreeGroup F;
  cout << "Enter a free group: ";
  errMsg = cin >> F;
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;

  GAMatrixBurau solverT(c, F); 
  GAFastBurMat solver(c, F); 


  Word solution = solver.getSolution(&cout);
  cout << "The solution is ";
  F.printWord(cout,solution);
  cout << endl;

  /*
  BurauPoly p(1),q(p);
  cout << "p = " << p << endl;
  p.multByIX();
  cout << "p*x^-1 = " << p << endl;
  q.sub(p);
  cout << "p*(1-x^-1) = " << q << endl;
  */
}

