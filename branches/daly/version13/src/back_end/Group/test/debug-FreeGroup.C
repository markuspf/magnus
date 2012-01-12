// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Informanl test of some methods of FreeGroup class
//
// Principal Author: Dmitry Bormotov
//
// Status: 
//
// Revision History:
//


#include "FreeGroup.h"


void main()
{    
  cout << "Enter a free group:" << endl;
  FreeGroup F;
  Chars errMsg = cin >> F;

  if ( errMsg.length() > 0 ) {
    cout << errMsg << endl << endl;
    return 1;
  }

  cout << endl << "You entered:" << endl << F << endl << endl;

  bool quit = false;
  while( !quit ) {

// -------------------------- getN_thElement() ----------------------------- //
    
    cout << "Enter the number n: ";
    int n;
    cin >> n;
    if( n < 0 )
      n = 0;
    cout << endl << "The " << n << "-th element of a free group is ";
    F.printWord(cout, F.getN_thElement(n));
    cout << endl << endl;
    
// -------------------------- the end of tests ----------------------------- //

    if( !quit ) {
      char reply;
      cout << "Repeat (y/n)? ";
      do {
	cin >> reply;
	reply = tolower(reply);
      } while( reply != 'y' && reply != 'n' );
      if( reply == 'n' ) quit = true;
    }
  }
}

