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


#include "GAFastBurMat.h"
#include "GAMatrixBurau.h"
#include "Generator.h"
#include "Vector.h"
#include "FreeGroup.h"

const int len = 5;
const int n = 6; //numOfGens+1
const int numOfGens = 5;
int v[100];

void destroyMatrix( BurauPoly** R )
{
  for( int i = 0; i < n; ++i )
    delete [] R[i];
  delete [] R;
}

BurauPoly** productMatrix( const PM* pm )
{
  Word u = ((PMDebornoyWord*)pm)->getPMWord().getWord();
  BurauPoly **R = new BurauPoly*[n];
  for( int i = 0; i < n; ++i ) {
    R[i] = new BurauPoly[n];
    R[i][i] = 1;
  }

  int uLen = u.length();
  for( int i = 0; i < uLen; ++i ) {
    int g = ord(u[i]);
    if( g > 0 ) {

      // R = R * bgens[g-1];
      for( int k = 0; k < n; ++k ) {
	BurauPoly t = R[k][g-1];
	t.multByX();
	R[k][g-1].sub(t);
	R[k][g-1].add(R[k][g]);
	R[k][g] = t;
      }
    }
    else {

      // R = R * bgensI[-g-1];
      for( int k = 0; k < n; ++k ) {
	BurauPoly t = R[k][-g];
	t.multByIX();
	R[k][-g].sub(t);
	R[k][-g].add(R[k][-g-1]);
	R[k][-g-1] = t;
      }
    }
  }
  return R;
}

int fitness( const PM* pm )
{
  BurauPoly **R = productMatrix(pm);
  int count = 0;
  for( int k = 0; k < n; ++k )
    for( int l = 0; l < n; ++l )
      if( k != l && !R[k][l].zero() )
	++count;
  
  destroyMatrix(R);
  return count; 
}

void processV( )
{
  VectorOf<Generator> vg(len);
  for( int i = 0; i < len; ++i )
    vg[i] = v[i];
  Word y(vg);
  
  Word a = Word(Generator(1));
  Word b = (y * Word(Generator(2)) * y.inverse()).freelyReduce();
  Word w = (a * b * a.inverse() * b.inverse()).freelyReduce();
  /*
  int wLen = w.length();
  for( int i = 0; i < wLen; ++i )
    if( w[i] == Generator(-1) ) 
      return;
  */
  UniformRandom r;
  PMWordConfig PMWC(r,numOfGens,44,false);
  PMDebornoyWord pm(PMWC,w);
  /*
  cout << "Matrix: " << endl;
  printMatrix( productMatrix( &pm ) ); 
  cout << endl;
  */
  if( fitness(&pm) == 0 )
    cout << w << endl;

  /*
  for( int i = 0; i < len; ++i )
    cout << " " << v[i];
  cout << endl;
  */
}

void nextLetter( int ind )
{
  if( ind >= len )
    processV();

  else if( ind == 0 )
    //for( int i = 1; i <= numOfGens; ++i ) {
    for( int i = 2; i <= 2; ++i ) {
      v[ind] = i;
      nextLetter(ind+1);
      if ( i != 1 ) {
	v[ind] = -i;
	nextLetter(ind+1);
      }
    }

  else {
    int p = v[ind-1];
    if( abs(p) == 1 ) {
      v[ind] = p;
      nextLetter(ind+1);
      return;
    }

    if( ind == len - 1 ) {
      v[ind] = 1;
      nextLetter(ind+1);
      v[ind] = -1;
      nextLetter(ind+1);
      return;
    }

    for( int i = 1; i <= numOfGens; ++i ) {
      if( i != -p && abs( i - abs(p) ) <= 2 ) {
	v[ind] = i;
	nextLetter(ind+1);
      }
      if( -i != -p && abs( i - abs(p) ) <= 2 ) {     
	v[ind] = -i;
	nextLetter(ind+1);
      }
    }
  }

}

int main ( )
{
  Map M;
  GAConfig c(50,-1,100,100,GAConfig::STRONG,true); 
  FreeGroup F;
  GAMatrixBurau solverT(c, F); 
  GAFastBurMat solver(c, F); 
 
  nextLetter(0);
}


