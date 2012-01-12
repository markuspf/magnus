// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Vladimir's change to Wada's experiments.
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "GAMatrixBurau.h"
#include "PBTree.h"

#include "Int2.h"


int main ( )
{
  Matrix< Polynomial<Integer> > I(4);
  Polynomial<Integer> p("1-x1,");

  int s;
  cout << "Enter s: ";
  cin >> s;
  cout << endl << endl;

  const int numOfGens = 5;
  VectorOf<Chars> v(numOfGens);
  v[0] = "x0"; 
  v[1] = "x1"; 
  v[2] = "x2"; 
  v[3] = "x3"; 
  v[4] = "x4"; 

  FreeGroup F(v);
  
  VectorOf<Chars> v2(numOfGens-2); 
  for( int i = 0; i < numOfGens-2; ++i )
    v2[i] = v[i];

  FreeGroup F2(v2);

  
  VectorOf<Word> id(numOfGens);
  for( int i = 0; i < numOfGens; ++i )
    id[i] = Word(Generator(i+1));
  
  VectorOf<Word> b2(id);
  VectorOf<Word> b1(id);

  --s;
  int maxLen = 0;
  
  while( true ) {
    
    ++s;
    cout << " s = " << s << " ..." << endl;
    Integer c = 0;

    for( int i = 1; i <= s; ++i )
      for( int j = 1; j <= s - i - 1; ++j ) {
	int k = s - i - j;
	++c;
	b1[0] = F2.getN_thElement(i);
	b1[1] = F2.getN_thElement(j);
	b1[2] = F2.getN_thElement(k);
	Map M1(F,F,b1);

	for( int x = 2; x < numOfGens; ++x ) {
	  b2[x] = b1[x-2];
	  int len = b2[x].length();
	  for( int y = 0; y < len; ++y ) {
	    int g = ord(b2[x][y]);
	    if( g > 0 )
	      b2[x][y] = Generator(g + 2);
	    else
	      b2[x][y] = Generator(g - 2);
	  }
        }
	Map M2(F,F,b2);

	/*
	int len = b1[0].length() + b1[1].length() + b1[2].length();
	if( len > maxLen ) {
	  maxLen = len;
	  cout << "*** Total length: " << maxLen << endl << endl;
	}
	*/































































































































































































	//cout << "M1 = " << M1 << endl;
	//cout << "M2 = " << M2 << endl;

	if( (M1 | M2 | M1) == (M2 | M1 | M2) ) {
	  cout << "Solution found: " << endl;
	  cout << "u = ";
	  F.printWord(cout,b1[0]);
	  cout << endl;
	  cout << "v = ";
	  F.printWord(cout,b1[1]);
	  cout << endl;
	  cout << "w = ";
	  F.printWord(cout,b1[2]);
	  cout << endl;
	  cout << "Checking whether this is auto: ";
	  if( F.isAutomorphism(b1) )
	    cout << "YES";
	  else
	    cout << "NO";
	  cout << endl << endl;
	}
      }

    cout << " s: " << s << "  # of elements: " << c << endl;
  }
}


