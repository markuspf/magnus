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
#include "GAEquationSolver.h"
#include "EquationParser.h"
#include "File.h"

double diffTime(timeval t2, timeval t1)
{
  double result;
  long s = t2.tv_sec - t1.tv_sec;
  result = (t2.tv_usec - t1.tv_usec) * 0.000001 + s;
  //if( t2.tv_usec < t1.tv_usec ) --result;
  return result;
}

int main ( )
{
  Chars errMsg;
  GHNConfig c;
  //cout << c << endl;

  ifstream in("../ge.in");
  in >> c;
  cout << c << endl;

  FreeGroup F;
  cout << "Enter a free group: ";
  errMsg = cin >> F;
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;

  VectorOf<Chars> empty, varNames; 
  EquationParser p(cin);
  cout << "Enter a graphic equation: ";
  Word equation = p.parseEquation(empty, varNames, errMsg);
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

  Chars name;
  cout << "Enter a file name prefix to save populations: ";
  cin >> name;
  cout << endl;
 
  /*
  FreeGroup G(names);
  int numOfVars = names.length();

  cout << "Equation: ";
  G.printWord(cout,pattern);
  cout << endl;
  cout << "w = " << w << endl;
  cout << "Names: " << names << endl;
  cout << "NumOfVars: " << numOfVars << endl;
  */
  timeval t1,t2;
  GraphicEquationSolver solver(F,varNames,equation,c,name);
  gettimeofday(&t1, NULL);
  Map solution = solver.getSolution(w,&cout);
  gettimeofday(&t2, NULL);
  cout << "The solution is " << solution << endl;
  cout << "Time elapsed: " << diffTime(t2,t1) << endl;
}





