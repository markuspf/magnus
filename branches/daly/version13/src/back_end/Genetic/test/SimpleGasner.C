// Copyright (C) 1998 The New York Group Theory Cooperative
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
#include "BraidGroupToolkit.h"

int main ( )
{
  Chars errMsg;
  GHNConfig c;
  //cout << c << endl;

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
 
  int count = 0, minMonCount = MAXINT;
  SetOf<Map> S;

  while( true ) {

    GAEquationSolver solver(G,numOfVars,c);
    Map solution = solver.getSolution(w,&cout);
    cout << "The solution is " << solution << endl;
    ++count;
    int oldCard = S.cardinality();
    S |= solution;
    cout << "Iterations: " << count << "   Different solutions: " 
	 << S.cardinality() << endl;
    
    if( S.cardinality() > oldCard ) {
      
      cout << "Fox derivatives: " << endl;
      BraidGroupToolkit<Integer> BGT;
      int numOfGens = F.numberOfGenerators();
      int monCount = 0;
      for( int v = 0; v < numOfVars; ++v )
	for( int g = 1; g <= numOfGens; ++g ) {
	  Polynomial<Integer> P 
	    = BGT.FoxDerivative(solution.generatingImages(v), g);
	  cout << "Dy" << v+1 << "/" << "x" << g << ": " << P;
	  PolynomialIterator<Integer> I(P);
	  for( ; !I.done(); I.next() )
	    ++monCount;
	}
      cout << "Total amount of monomials: " << monCount << endl;
      if( monCount == 0 ) {

	// exclude trivial examples
	bool allZeros = true;
	for( int v = 0; v < numOfVars; ++v )
	  if( solution.generatingImages(v).length() != 0 ) {
	    allZeros = false;
	    break;
	  }
	if( allZeros ) {
	  cout << "Trivial counterexample has been found" << endl;
	  continue;
	}
	else {
	  cout << "Counterexample has been found !" << endl;
	  return 0;
	}
      }

      if( minMonCount > monCount ) 
	minMonCount = monCount;
      cout << "Minimal total amount of monomials: " << minMonCount 
	   << endl << endl;
    }
  }
}

