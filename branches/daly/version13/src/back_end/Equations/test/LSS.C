// Copyright (C) 1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Trying a series of examples described by Lyndon and Shupp in order
//           to find a solution of genus > 1.
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
#include "DGESolver.h"
#include "EquationParser.h"
#include "Automorphism.h"

Word initWord(const Generator* p, int len) {
  VectorOf<Generator> v(len);
  for( int i = 0; i < len; i ++ ) v[i] = *p++;
  return Word(v);
}

void main ( )
{
  VectorOf<Chars> v(2);
  v[0] = "a"; v[1] = "b";
  FreeGroup F(v);

  const Generator t11[] = { 1,1,2,2,-1};
  const Generator t12[] = { 1,-2,-1};

  const Generator t21[] = { 1,1,2,-1,-1};
  const Generator t22[] = { 1 };

  const Generator t31[] = { -2,-2,1,2,2 };
  const Generator t32[] = { -2,-2,-1,-1,2,1,1,2,2};

  Word w11 = initWord(t11, 5);
  Word w12 = initWord(t12, 3);
  Word w21 = initWord(t21, 5);
  Word w22 = initWord(t22, 1);
  Word w31 = initWord(t31, 5);
  Word w32 = initWord(t32, 9);
 
  Map M[3], IM[3];

  VectorOf<Word> v1(2);
  v1[0] = w11; v1[1] = w12;
  M[0] = Map(F,F,v1);

  VectorOf<Word> v2(2);
  v2[0] = w21; v2[1] = w22;
  M[1] = Map(F,F,v2);

  VectorOf<Word> v3(2);
  v3[0] = w31; v3[1] = w32;
  M[2] = Map(F,F,v3);

  for( int k = 0; k < 3; ++k )
    IM[k] = Automorphism(M[k]).inverse();

  // Generate all consequences of M[0],M[1],M[2]

  VectorOf<Chars> names(2);
  names[0] = "t1";
  names[1] = "t2";
  //names[2] = "t3";
  FreeGroup G(names);
  
  SetOf<Word> S,T;
  int n = 0;
  while( true ) {

    Word elt = G.getN_thElement(++n);
    int len = elt.length();

    // Compute the image defined by elt

    Word x(Generator(1));
    Word y(Generator(2));

    for( int i = 0; i < len; ++i ) {
      
      int gen = ord(elt[i]);
      if( gen > 0 ) {
	x = M[gen-1].imageOf(x).freelyReduce();
	y = M[gen-1].imageOf(y).freelyReduce();
      } else {
	x = IM[-gen-1].imageOf(x).freelyReduce();
	y = IM[-gen-1].imageOf(y).freelyReduce();
      } 
    }
      
    Word w = (Word(Generator(1)) * Word(Generator(2)) * y.inverse() * x.inverse())
      .freelyReduce();
    
    int c1 = S.cardinality();
    S |= w;
    int c2 = S.cardinality();
    if( c2 > c1 )
      T |= elt;
   
    Word p1,p2;
    //cout << n << ": w = ";
    //G.printWord(cout, elt);
    //cout << " = ";
    //F.printWord(cout, w);
    //cout << " is ";
    bool answer = F.isCommutator(w,p1,p2);
    //if( !answer ) cout << "NOT ";
    //cout << "a commutator" << endl;
    if( !answer ) return;
    if( n % 1000 == 0 ) cout << "N: " << n << "  Cardinality of S: " 
			    << S.cardinality() << endl;
    if( n % 10000 == 0 ) { 
      cout << "S: " << endl;
      SetIterator<Word> I(S);
      for( ; !I.done(); I.next() ) {
	F.printWord(cout,Word(I.value()));
	cout << endl;
      }
      cout << endl;

      cout << "T: " << endl;
      SetIterator<Word> I2(T);
      for( ; !I2.done(); I2.next() ) {
	G.printWord(cout,Word(I2.value()));
	cout << endl;
      }
      cout << endl;

    }
  }
}






