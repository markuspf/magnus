// Copyright (C) 1999 The New York Group Theory Cooperative
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
#include "Automorphism.h"
#include "FreeGroup.h"
#include "strstream.h"
#include "GAMatrixBurau.h"
#include "Config.h"

int main ( )
{
  Chars errMsg;
  FreeGroup F;

  GAConfig c;
  GAMatrixBurau solver(c, F); 


  cout << "Enter a free group: ";
  errMsg = cin >> F;
  if( errMsg.length() > 0 ) {
    cout << errMsg;
    exit(0);
  }
  cout << endl;

  cout << "Enter a word: ";
  Word w = F.readWord(cin, errMsg);
  if( errMsg.length() > 0 ) {
    cout << errMsg << endl;
    exit(0);
  }
  
  int fNumOfGens = F.numberOfGenerators();
  Map M1[fNumOfGens], M2[fNumOfGens];
  int gNumOfGens = fNumOfGens + 1;
  strstream o;
  o << "<";
  for( int i = 0; i < gNumOfGens; ++i ) {
    o << "x" << i+1;
    if( i != gNumOfGens-1 ) 
      o << ",";
  }
  o << ">" << ends;
  FreeGroup G;
  o >> G;
  //cout << "G = " << G << endl;

  for( int i = 0; i < fNumOfGens; ++i ) {
    VectorOf<Word> im(gNumOfGens);
    for( int j = 0; j < gNumOfGens; ++j )
      im[j] = Word(Generator(j+1));
    im[i] = im[i] * im[i+1] * im[i].inverse();
    im[i+1] = Word(Generator(i+1));
    M1[i] = Map(G,G,im);
    cout << "M1[" << i << "] = " << M1[i] << endl;
    M2[i] = Automorphism(M1[i]).inverse();
    cout << "M2[" << i << "] = " << M2[i] << endl;
 }

  VectorOf<Word> im(gNumOfGens);
  for( int j = 0; j < gNumOfGens; ++j )
    im[j] = Word(Generator(j+1));
  Map M(G,G,im);  
  w = w.freelyReduce();
  int wLen = w.length();
  for( int i = 0; i < wLen; ++i ) {
    int g = ord(w[i]);
    cout << "i = " << i << "   g = " << g << endl;
    if( g > 0 )
      M = M | M1[g-1];
    else
      M = M | M2[-g-1];

    cout << "length: ";
    for( int i = 0; i < gNumOfGens; ++i )
      cout << " " << M.generatingImages(i).length();
    cout << endl;

    //cout << "M: " << M << endl; 
  }
  //cout << "Resulting auto: " << M << endl; 

  
  cout << endl << endl << "Lengths: " << endl;
  for( int len = 1; len <= gNumOfGens; ++len )
    for( int i = 0; i < gNumOfGens-len+1; ++i ) {
      Word w;
      for( int j = 0; j < len; ++j ) {
	w = (w * M.generatingImages(i+j)).freelyReduce();
	if( j != 0 ) cout << " * ";
	cout << "y" << i+1+j;
      }
      cout << " :  " << w.length() << endl;
    }
}

