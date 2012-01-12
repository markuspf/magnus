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


#include "GeneralWhitehead.h"

int main ( )
{
  FreeGroup F(2);
  int n = 1;
  int curLen = 1;
  int sum = 0;
  int numOfElts = 0;

  cout << "Free Group F = " << F << endl;
  
  while( true ) {
    
    Word w = F.getN_thElement(n++);
    int wLen = w.length();
    Word r = w.freelyReduce();
    if ( r.length() < wLen ) continue;
    
    if( wLen > curLen) {
      cout << "Length: " << curLen << "   "
	   << "Number of elements: " << numOfElts << "   "
	   << "Primitive elements: " << sum << endl;;
      curLen = wLen;
      sum = 0;
      numOfElts = 0;
    }

    ++numOfElts;
    
    GeneralWhitehead GW(F);
    GW.startComputation(w);
    while( !GW.continueComputation() );
    if( GW.extendsToFreeBasis() ) ++sum;
  }
}

