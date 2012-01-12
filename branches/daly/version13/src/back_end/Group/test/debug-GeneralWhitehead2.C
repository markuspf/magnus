// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test of class GeneralWhitehead.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//

#define debug_GeneralWhitehead

#include "GeneralWhitehead.h"

int main ( )
{
  cout << "Enter a free group presentation: " << endl << endl;

  FreeGroup F;
  Chars errMsg = cin >> F;

  if ( errMsg.length() > 0 ) {
    cout << errMsg << endl << endl;
    return 1;
  }

  cout << endl << "You entered: " << endl << endl;
  cout << F << endl << endl;

  bool quit = false;
  while( !quit ) {
    

// -------------------- the extendsToFreeBasis() test --------------------- //
    

    cout << "Enter a vector of words: " << endl;
    VectorOf<Word> V = F.readVectorOfWords(cin, errMsg);

    if( errMsg.length() > 0 ) {
      cout << errMsg << endl;
      return(1);
    }

    cout << "Trying to define, does this set extend to a free basis or not..."
	 << endl << endl;

    GeneralWhitehead GW(F, true);

    cout << "The file name: " << GW.getFileName() << endl << endl;
    GW.startComputation(V);

    int arc = 0;
    while ( !GW.continueComputation() )
      cout << "Arc " << arc++ << endl;
    
    if( GW.extendsToFreeBasis() ) {
      cout << "It does extend." << endl << endl;
      cout << "Automorphism: " << endl << GW.getAutomorphism();
    }
    else
      cout << "It doesn't extend.";
    cout << endl << endl;

    char reply;
    cout << "Repeat (y/n)? ";
    do {
      cin >> reply;
      reply = tolower(reply);
    } while( reply != 'y' && reply != 'n' );
    if( reply == 'n' ) quit = true;
  }
}
