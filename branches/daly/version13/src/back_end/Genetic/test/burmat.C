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
#include "File.h"


int main ( )
{
  Map M;
  Chars errMsg;
  GAConfig c(50,-1,100,100,GAConfig::SIMPLE,true); 
  cout << c << endl;
 
  FreeGroup F;
  cout << "Enter a free group: ";
  errMsg = cin >> F;
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;

  GAMatrixBurau solver(c, F); 
  Word solution = solver.getSolution(&cout);

  cout << "The solution is ";
  F.printWord(cout,solution);
  cout << endl;
}

