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
#include "EquationParser.h"
#include "File.h"
//#include "GAMatrixBurau.h"
#include "Polynomial.h"
#include "Map.h"


int main ( )
{
  Polynomial<Integer> p1;
  Monomial<Integer> m;
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

  VectorOf<Chars> names; 
  EquationParser p(cin);
  cout << "Enter an equation: ";
  Word w = p.parseEquation(F.namesOfGenerators(), names, errMsg);
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;
  
  FreeGroup G(names);
  int numOfVars = names.length() - F.numberOfGenerators();
 
  cout << "Equation: ";
  G.printWord(cout,w);
  cout << endl;
  cout << "w = " << w << endl;
  cout << "Names: " << names << endl;
  cout << "NumOfVars: " << numOfVars << endl;
}

