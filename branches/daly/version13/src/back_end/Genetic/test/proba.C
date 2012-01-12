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


#include "GAMatrixBurau.h"
#include "PBTree.h"
#include "Int2.h"


int main ( )
{
  Word w;
  Map Ma;

  Polynomial<Integer> p("1-x1,");
  Polynomial<Integer> q("1-x1,");
  //cout << " p*q = " << p*q << endl;

  int n = 6;
  Matrix< Polynomial<Integer> > I(n);
  for( int k = 0; k < n; ++k )
    for( int l = 0; l < n; ++l )
      if( k != l ) 
	I[k][l] = "0,";
      else
	I[k][l] = "1,";

  Matrix< Polynomial<Integer> > M(I),N(I);
  int i = 0;
  M[i][i] = "1 - x1,";
  M[i][i+1] = "x1,";
  M[i+1][i] = "1,";
  M[i+1][i+1] = "0,";

  //M[0][0] = p;
  //N[0][0] = q;

  PBTree< Monomial<Integer>,Monomial<Integer> > T;
  Monomial<Integer> m1("x1,");
  Monomial<Integer> m2(1);
  cout << "m1 = " << m1 << "   m2 = " << m2 << endl;

  while( true ) {
    T.insert(m1,m1);
    T.insert(m2,m2);
    T.deleteAll();
  }

  cout << "N*M = " << N << endl;
}


