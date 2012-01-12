// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of classes RSGenerator, RSWord
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// Special Notes:
//


#include "RSWord.h"

//#define debug_RSWord


// ------------------------------ RSWord ----------------------------------- //


RSWord::RSWord( const FreeGroup& F, const RSWord& w )
  : theGroup( F ), theWord( w.length() )
{
  for( int i = 0; i < w.length(); ++i )
    theWord[i] = w[i];
}


RSWord& RSWord::operator = ( const RSWord& w )
{ 
  theGroup = w.theGroup;
  theWord = w.theWord;
  return *this;
}


bool RSWord::operator == ( const RSWord& w ) const
{ 
  return theGroup == w.theGroup && theWord == w.theWord;
}


RSWord::RSWord( const FreeGroup& F, const Word& w ) : theGroup( F )
{
  // Convert w to a form where all segment like 't t t' converted to
  // 't^3', in other words, in the result, in any sequence of two
  // generators 'g1 g2', g1 and g2 are diffirent.

  int wLen = w.length();
  VectorOf<RSGenerator> res(wLen);
  Generator g;
  int i = 0, resI = -1;

  while( i < wLen ) {
    g = w[i];

    int pow = 1;
    while( ++i < wLen && g == w[i] )
      ++pow;
    
    if( ord(g) < 0 ) {
      g = -ord(g); pow = -pow;
    }

    res[++resI] = RSGenerator(g, 0, pow);
  }
  
  theWord = VectorOf<RSGenerator>(resI+1);
  for( int j = 0; j <= resI; ++j )
    theWord[j] = res[j];
  
#ifdef debug_RSWord
  printOn(cout);
#endif
  
  // Now we apply RS moves: we replace 't^-i g t^i' by 'g_i'
  

  int theLen = theWord.length();
  res =  VectorOf<RSGenerator>(theLen);
  int theI = 0;
  resI = 0;
  
  while( theI < theLen ) {
    if( theWord[theI].gen != 1 ) {
      res[resI++] = theWord[theI];
      ++theI;
    }
    else if ( ++theI >= theLen )
      error("RSWord::RSWord(...) : exponent sum of t's is not zero");
    else {

      int pow = theWord[theI-1].power;
      RSGenerator RSG = RSGenerator(theWord[theI].gen, -pow, 
				    theWord[theI].power);
      if( resI != 0 && res[resI-1].gen == RSG.gen 
	  &&  res[resI-1].index == RSG.index )
	res[resI-1].power += RSG.power;
      else
	res[resI++] = RSG;
      
      theWord[theI] = RSGenerator( Generator(1), 0, pow );
      if( theI+1 >= theLen )
	continue;
      if( theWord[theI+1].gen == 1 ) {
	++theI;
	theWord[theI].power += theWord[theI-1].power;
	if( theWord[theI].power == 0 )
	  ++theI;
      }
    }
  }
   
  theWord = VectorOf<RSGenerator>(resI);
  for( int j = 0; j < resI; ++j )
    theWord[j] = res[j];
}


void RSWord::printOn( ostream& ostr ) const
{
  for( int i = 0; i < length(); ++i ) { 
    ostr << theGroup.nameOfGenerator(theWord[i].gen)
	 << "_" << theWord[i].index;
    if( theWord[i].power != 1 )
      ostr << "^" << theWord[i].power;
    ostr << " ";
  }
  ostr << endl;
}
