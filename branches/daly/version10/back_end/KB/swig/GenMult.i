///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "GenMult.h"
%}


class GenMult : public FSA {
// this should surely be derived from DFSA but I can't get the
// constructor to work except by doing this
public:
  GenMult( );

  %name(GenMultFromVector) GenMult( const VectorOf<Chars> &  genNames ) ;

  %name(GenMultFromVectorWordOrder) GenMult( const VectorOf<Chars> & genNames, const WordOrder & word_order) ;

/*
protected:
  // Need to shadow inherited look() and change(), to get right ptr types.
  const GenMultRep *look() const { 
    return (GenMultRep*)FSA::look(); }
  GenMultRep *change() { 
    return (GenMultRep*)FSA::change(); }
*/
};

