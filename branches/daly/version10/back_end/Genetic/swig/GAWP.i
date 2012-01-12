///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "GAWP.h"
%}

// ------------------------------- GAWP ------------------------------------ //


class GAWP
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GAWP( const FPGroup& G, const GHNConfig& config ) ;
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

//  Trichotomy isTrivial( const Word& u, ostream& out );
  // returns yes if the genetic algorithm could prove that u
  // is trivial; returns dontknow after computing all generations


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

/*
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int fitness( const Word& u ) const;
  
  Word mutate( const Word& u );

  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  FPGroup theGroup;
  GHNConfig cfg;
  Word w;
  int wLen;
  SetIterator<Word> I;
  UniformRandom r;
*/
// Trichotomy isTrivial( const Word& u, ostream& out );
  %addmethods{
     Trichotomy isTrivial( const Word& u, ostream* out=NULL ){
         return self->isTrivial(u,*out);
     }
  }

};


// ------------------------------- GAWP2 ----------------------------------- //


class GAWP2
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GAWP2( const FPGroup& G, const GHNConfig& config );
  
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Trichotomy isTrivial( const Word& u, ostream* out = NULL );
  // returns yes if the genetic algorithm could prove that u
  // is trivial; returns dontknow after computing all generations;
  // if out is not NULL it keeps details of the computation


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
/*

private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int fitness( const Word& u ) const;
  
  Word mutate( const Word& u );

  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  FPGroup theGroup;
  GHNConfig cfg;
  SetOf<Word> relators;
  Word w;
  int wLen;
  bool keepDetails;
  SetIterator<Word> I;
  UniformRandom r;
*/
};
