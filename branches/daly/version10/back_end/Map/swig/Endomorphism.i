
%module Magnus

%{
#include "Endomorphism.h"
%}


class Endomorphism:public Map{

public:

////////////////////////////////////////////////////////////////////
//                                                                //
// Constructors                                                   //
//                                                                //
////////////////////////////////////////////////////////////////////

	// no default constructor: a domain group must be given

	Endomorphism( const FGGroup& dom);
	// make trivial endomorphism on given group

	%name(EndomorphismFromWord) Endomorphism( const FGGroup& dom, const VectorOf<Word>& generatingImages );
	// an Endomorphism with explicit generatingImages

	%name(EndomorphismFromMap) Endomorphism( const Map& m );
  // cast construtor: to cast an arbitrary map as an endomorphism.
  // NB. This rewraps the unchanged representation, hence is in general
  // only useful for casting a map known to be an actual endomorphism.

	// copy constructor, operator=, and destructor supplied by compiler

////////////////////////////////////////////////////////////////////
//                                                                //
// Accessors / Modifiers                                          //
//                                                                //
////////////////////////////////////////////////////////////////////

	// inherited from the base class

	// const FGGroup& domain( ) const;
	// const FGGroup& range( ) const;
	// const VectorOf<Word>& generatingImages( ) const;
	// Word generatingImages( int i ) const;
	// void setGeneratingImages( const VectorOf<Word>& gi );
	// void setGeneratingImages( int i, const Word& e  );

  void makeIdentity();
	// turns the endomorphism into the identity automorphism. 

  void reduceGenImages() ;
	// freely reduce generating images to prevent its overgrowth 
	// in composition of maps. 
	//@ep Should be this done automatically?

////////////////////////////////////////////////////////////////////
//                                                                //
// Standard operations                                            //
//                                                                //
////////////////////////////////////////////////////////////////////

//  bool operator ==(const Endomorphism& e) const ;
	//@dp this operator should be defined in Map only. But now
	//    Map class has too simple of this one.

////////////////////////////////////////////////////////////////////
//                                                                //
// Mapping methods                                                //
//                                                                //
////////////////////////////////////////////////////////////////////

	// inherited from the base class

//    friend CondParamLvalue<Map,Word,Generator>::operator Word ( ) const;
//    friend Word& CondParamLvalue<Map,Word,Generator>::operator =
//        ( const Word& ) const;
//    Word imageOf( const Word& w ) const;
//    Word imageOf( const Generator& g ) const;
//    CondParamLvalue<Map,Word,Generator> imageOf( const Generator& g );
//    Word operator()( const Word& w ) const;
//    Word operator()( const Generator& g ) const;
//    CondParamLvalue<Map,Word,Generator> operator()( const Generator& g )
//        { return imageOf(g); }

// computing images:

//    Elt evalImage( const Word& w ) const;
//    Elt postEvalImage( const Word& w ) const;


////////////////////////////////////////////////////////////////////
//                                                                //
// Mapping properties                                             //
//                                                                //
////////////////////////////////////////////////////////////////////

//    Trichotomy extendsToHom( ) const;

//    Trichotomy isMono( ) const;
//    Trichotomy isEpi( ) const;
//    Trichotomy isIso( ) const;
//    Trichotomy isTrivialMap( ) const; // @stc impl. tmp.

////////////////////////////////////////////////////////////////////
//                                                                //
// Debugging stuff                                                //
//                                                                //
////////////////////////////////////////////////////////////////////

#ifdef DEBUG

//	friend int main( );

	bool consistent( );

#endif
};



