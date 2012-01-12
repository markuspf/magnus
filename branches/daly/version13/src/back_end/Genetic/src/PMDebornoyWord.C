// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of PMDebornoyWord class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "PMDebornoyWord.h"


// --------------------------- PMDebornoyWord ------------------------------ //


PM* PMDebornoyWord::crossover( const PM* w )
{
  PMDebornoyWord* pm2 = (PMDebornoyWord*)w;
  PMWord* r = 0;
  do {
    delete r;
    PMWord t = pm2->getPMWord();
    r = (PMWord*)pm.crossover(&t);
  } while( !hasFirstGen(*r) );

  PMDebornoyWord *d = new PMDebornoyWord(cfg, *r);
  delete r;
  return d;
}


PM* PMDebornoyWord::mutate( )
{
  PMWord *r = 0;
  do {
    delete r;
    r = (PMWord*)pm.mutate();
  } while( !hasFirstGen(*r) );

  Word w = r->getWord();
  int wLen = w.length();
  for( int i = 0; i < wLen; ++i ) 
    if( ord(w[i]) == -1 )
      w[i] = 1;
  delete r;
  PMWord p(pm.getCfg(), w);
  PMDebornoyWord *d = new PMDebornoyWord(cfg,p);
  return d;
}


bool PMDebornoyWord::hasFirstGen( const PMWord& r )
{
  Word w = r.getWord();  
  int wLen = w.length();
  for( int i = 0; i < wLen; ++i ) 
    if( ord(w[i]) == 1 )
      return true;
  return false;
}


PM* PMDebornoyWord::randomPM( )
{
  PMWord *r = (PMWord*)pm.randomPM();
  Word w = r->getWord();
  int wLen = w.length(), count = 0;
  for( int i = 0; i < wLen; ++i ) {
    int g = ord(w[i]);
    if( g == -1 ) {
      w[i] = -g;
      ++count;
    }
  }

  if( count == 0 ) {
    int num = min( wLen, int(pm.getCfg().rng().rand() * (wLen+1)) );
    Word a,b;
    if( num > 0 )
      a = w.initialSegment(num);
    if( num < wLen ) 
      b =  w.terminalSegment(wLen-num);
    w = a * Word(Generator(1)) * b;
  }

  return new PMDebornoyWord(cfg, w);
}

void PMDebornoyWord::printOn( ostream& ostr ) const 
{ 
  cfg.getFreeGroup().printWord(ostr,pm.getWord());
}
