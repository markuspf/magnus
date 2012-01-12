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
#include "GABraid.h"
#include "EquationParser.h"
#include "File.h"


void SComGens( const FreeGroup& F, FreeGroup& B, VectorOf<Word>& gens )
{
  int numOfGens = F.numberOfGenerators();
  int count = 0;
  for( int i = 0; i < numOfGens; ++i )
    for( int j = i+1; j < numOfGens; ++j )
      ++count;
  VectorOf<Chars> v(count);
  for( int i = 0; i < count; ++i ) {
    ostrstream o;
    o << "g" << i+1 << ends;
    v[i] = o.str();
  }

  B = FreeGroup(v);
  gens = VectorOf<Word>(count);
  int c = 0;
  
  for( int i = 0; i < numOfGens; ++i )
    for( int j = i+1; j < numOfGens; ++j )
      gens[c++] = Word(Generator(i+1)) * Word(Generator(j+1)).inverse();
}


int main ( )
{
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
 
  /*
  cout << "Equation: ";
  G.printWord(cout,w);
  cout << endl;
  cout << "w = " << w << endl;
  cout << "Names: " << names << endl;
  cout << "NumOfVars: " << numOfVars << endl;
  */

  FreeGroup B;
  VectorOf<Word> gens;
  SComGens(F,B,gens);
  cout << " B = " << B << endl 
       << " gens: " << gens << endl;

  FreeGroup SC( concatenate(B.namesOfGenerators(),F.namesOfGenerators()) );
  cout << " SC = " << SC << endl; 

  GABurauSolver solver(c, G, numOfVars, B, gens, SC ); 
  Map solution = solver.getSolution(w,&cout);

  cout << "The solution is " << solution << endl;
}

