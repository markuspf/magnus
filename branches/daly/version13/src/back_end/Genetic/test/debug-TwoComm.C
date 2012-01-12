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
#include "TwoCommSolver.h"
#include "EquationParser.h"
#include "File.h"

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
  cout << endl;

   cout << "Enter a word in F: ";
  WordParser P(cin);
  Word w = P.parseWordVerbatim( F.namesOfGenerators(), errMsg);  
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;

  TwoCommSolver solver(F);
  Word x1,x2,y1,y2;
  if( solver.isProductOfTwoComms(w,x1,x2,y1,y2,&cout) ) {
    cout << "It is a product of two commutators:" << endl
	 << "[";
    F.printWord(cout, x1);
    cout << ", ";
    F.printWord(cout, x2);
    cout << "] [";
    F.printWord(cout, y1);
    cout << ", ";
    F.printWord(cout, y2);
    cout << "]" << endl;
  }

  cout << "Checking... ";
  if( (w.inverse() * x1.inverse() * x2.inverse() * x1 * x2 
       * y1.inverse() * y2.inverse() * y1 * y2).freelyReduce().length() == 0 )
    cout << "Correct";
  else
    cout << "Incorrect";
  cout << endl;
}





