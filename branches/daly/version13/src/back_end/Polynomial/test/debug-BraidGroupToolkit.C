// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Informal tests of class BraidGroupToolkit
//
// Principal Authors: Dmitry Bormotov
//


#include "FreeGroup.h"
#include "BraidGroupToolkit.h"


void main( )
{
  FreeGroup G;
  cout << "Enter a free group: ";
  Chars errMsg = cin >> G;
  if( errMsg.length() > 0 ) {
    cout << errMsg << endl;
    exit(-1);
  }
  
  cout << "Type a word: ";
  Word w = G.readWord(cin, errMsg);
  if( errMsg.length() > 0 ) {
    cout << errMsg << endl;
    exit(0);
  }

  cout << "You typed: ";
  G.printWord(cout,w);
  cout << endl;

  /*
  int n;
  cout << "Enter the number of the variable ( >= 1 ): ";
  cin >> n;
  if( n < 1 )
    error("Input error: the number must be >= 1");
  */

  BraidGroupToolkit<Integer> BGT;
  for( int n = 1; n <= G.numberOfGenerators(); ++n ) {
    Polynomial<Integer> P = BGT.FoxDerivative(w,n);
    cout << "Fox derivative (" << n << "): " << P << endl;
    /*
    cout << "Monomials: " << endl; 
    int c = 0;
    
    PolynomialIterator<Integer> I(P);
    for( ; !I.done(); I.next() ) {
      
      cout << ++c << ": " << I.getValue() << endl;
      
      int n = I.getValue().getNumberOfVariables();
      cout << "Coefficient: " << I.getValue().getCoefficient() << endl
	   << "Number of variables: " << n << endl
	   << "Powers: ";
      for( int i = 0; i < n; ++i)
	cout << I.getValue().getPowerOfVariable(i) << " ";
      cout << endl << endl;
    }
    */
  }
}

