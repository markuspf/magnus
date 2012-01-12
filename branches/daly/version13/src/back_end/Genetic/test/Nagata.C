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
//#include "FreeGroup.h"
#include "Config.h"
#include "RandomNumbers.h"
//#include "GAMatrixBurau.h"
//#include "GAFastBurMat.h"
#include "File.h"
#include "GANagata.h"


int main ( )
{
  Map M;
  Chars errMsg;
  GANagataConfig c; 
  cin >> c;
  cout << c << endl;
 
  GANagata solver(c); 

  Polynomial<Int2> solution = solver.getSolution(&cout);
  cout << "The solution is " << solution << endl;
}

