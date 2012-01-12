// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: An alternative way to solve the word problem for
//           free nilpotent groups
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include "PowerSeriesWP.h"
#include "Polynomial.h"


int main ( )
{
  Polynomial<Integer> p;
  Map M;
  FreeGroup F;
  cout << "Enter a free group: ";
  cin >> F;
  cout << endl;
  
  int c;
  cout << "Enter the nilpotency class: ";
  cin >> c;
  cout << endl;

  PowerSeriesWP PSWP(F,c);

  while( 1 ) {
    
    Chars err;
    Word w;
    cout << "Enter a word: ";
    w = F.readWord(cin, err);
    cout << endl << endl;
    
    cout << "You entered: F = " << c << " " << F << endl;
    cout << "w = ";
    F.printWord(cout, w);
    cout << endl;
    
    bool result = PSWP.isTrivial(w);
    
    cout << "The word w is ";
    if( !result) 
      cout << "NOT ";
    cout << "trivial." << endl << endl;
  }
}





