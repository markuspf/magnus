// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//

#include "RKBPackage.h"
#include "KBMachine.h"
#include "FPGroup.h"

int main ( )
{
  cout << "Enter a group presentation: " << endl << endl;

  FPGroup G;
  Chars errMsg = cin >> G;

  if ( errMsg.length() > 0 ) {
    cout << errMsg << endl << endl;
    return 1;
  }

    int maxRelLen = 0;
  SetIterator<Word> I( G.getRelators() );
  while ( !I.done() ) {
	 Word w = I.value();
	 int l = w.length();
	 if ( l > maxRelLen ) maxRelLen = l;
	 I.next();
  }
  rkbp_par = 2 * maxRelLen;

  // Make the RKBPackage:

  theRKBPackage = new RKBPackage( G.namesOfGenerators(), G.getRelators() );

  goodMood = theRKBPackage->sanityCheck();


}



