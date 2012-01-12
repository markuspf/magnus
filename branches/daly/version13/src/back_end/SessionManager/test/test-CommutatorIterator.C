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

#include "global.h"
#include "ExtendToHomProblem.h"

int main ( )
{
/*
  int numOfGens;
  cout << "Enter the number of generators: ";
  cin >> numOfGens;

  int length;
  cout << "Enter the length of commutators: ";
  cin >> length;

  cout << endl << endl;

  CommutatorIterator I(numOfGens, length);
  for( ; !I.done(); I.next() )
    cout << I.value() << endl;

  cout << endl;
*/

  cout << "Enter a free group presentation: " << endl << endl;

  FreeGroup F;
  Chars errMsg = cin >> F;

  if ( errMsg.length() > 0 ) {
    cout << errMsg << endl << endl;
    return 1;
  }

  int numOfGens;
  cout << "Enter the vector of words: ";
  VectorOf<Word> V = F.readVectorOfWords(cin, errMsg);

  if ( errMsg.length() > 0 ) {
    cout << errMsg << endl << endl;
    return 1;
  }


  int length;
  cout << "Enter the length of commutators: ";
  cin >> length;

  cout << endl << endl;

  CommutatorIterator I(V, length);
  for( ; !I.done(); I.next() )
    cout << I.value() << endl;

  cout << endl;
}



