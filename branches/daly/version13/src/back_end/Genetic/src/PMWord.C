// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of PMWord class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "PMWord.h"


// ------------------------------- PMWord ---------------------------------- //


PM* PMWord::crossover( const PM* w )
{
  Word res;
  do {
    UniformRandom& r = cfg.rng();
    Word v = ((PMWord*)w)->theWord; 
    int len1 = theWord.length();
    int len2 = v.length();
    Word a,b;
    if( len1 > 0 ) {
      int num1 = min(len1,int(r.rand() * (len1+1)));
      a = theWord.initialSegment(num1);
    }
    if( len2 > 0 ) {
      int num2 = min(len2,int(r.rand() * (len2+1)));
      b = v.terminalSegment(num2);
    }
    /*
    cout << "u: " << theWord << endl
	 << "v: " << v << endl
	 << "a: " << a << endl
	 << "b: " << b << endl << endl;
    */ 
    res = (a * b).freelyReduce();
  } while( !cfg.allowTrivialWords() && res.length() == 0 );

  return new PMWord(cfg, res);
}  

PM* PMWord::mutate( )
{
  Word w;
  do {
    UniformRandom& r = cfg.rng();
    w = theWord;
    int wLen = w.length();
    int num = min(wLen-1, int(r.rand() * wLen) );
    
    float op = r.rand();
    
    if( op <= 0.1 ) { // insert a new letter to the word (10% chance)
      num = min(wLen, int(r.rand() * (wLen+1)));
      Word a,b;
      if( num > 0 )
	a = w.initialSegment(num);
      if( num < wLen ) 
	b =  w.terminalSegment(wLen-num);
      w =  a * Word(Generator(randomGen())) * b;
    }
    
    else if( op <= 0.2 ) { // delete one letter (10% chance)
      if( wLen > 0 ) {
	Word a,b;
	if( num > 0 )
	  a = w.initialSegment(num);
	if( num < wLen - 1 ) 
	  b =  w.terminalSegment(wLen-1-num);
	w = a * b;
      }
    }
    
    else { // mutate one letter (80% chance)
      if( wLen > 0 )
	w[num] = Generator(randomGen());
    }
    
    w = w.freelyReduce();
  } while( !cfg.allowTrivialWords() && w.length() == 0 );
  
  return new PMWord(cfg, w );
}

int PMWord::randomGen( )
{
  UniformRandom& r = cfg.rng();
  int gen = min(cfg.getNumOfGens()-1, int(r.rand() * cfg.getNumOfGens())) + 1;
  return (r.rand() <= 0.5) ? gen : -gen;
}

PM* PMWord::randomPM( )
{
  UniformRandom& r = cfg.rng();
  Word w;
  do { 
    //int vLen =  int(r.rand() * cfg.getWordMaxLen()) + 1;
    int vLen = cfg.getWordMaxLen();
    VectorOf<Generator> v(vLen);
    for( int i = 0; i < vLen; ++i )
      do { 
	v[i] = randomGen();
      } while( i > 0 && ord(v[i]) == -ord(v[i-1]) ); 
    w = Word(v).freelyReduce();
  } while( w.length() == 0 );
  
  return new PMWord(cfg, w);
}
