
%module Magnus

%{
#include "Automorphism.h"
%}



class Automorphism :public Map{

public:

////////////////////////////////////////////////////////////////////
//                                                                //
// Constructors                                                   //
//                                                                //
////////////////////////////////////////////////////////////////////

  Automorphism( const FGGroup& dom);
	// make trivial Automorphism on given group

	%name(AutomorphismFromVector) Automorphism( const FGGroup& dom,	const VectorOf<Word>& generatingImages );
	// an Automorphism with explicit generatingImages

	%name(AutomorphismFromGenerator) Automorphism(FGGroup& dom, Generator x, Word& w);
	// maps x to w, the others to themselves

	%name(AutomorphismFromWord) Automorphism(FGGroup& dom, Word& w);
	// An `inner' endomorphism, i.e., x -> x^w for all generators x.

	%name(AutomorphismFromMap) Automorphism( const Map& m );
  // cast construtor: to cast an arbitrary map as an automorphism.
  // NB. This rewraps the unchanged representation, hence is in general
  // only useful for casting a map known to be an actual automorphism.

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
	// void setDomain( const FGGroup& g ); 
	// void setRange( const FGGroup& g );
	// void makeIdentity()
	// void reduceGenImages();

////////////////////////////////////////////////////////////////////
//                                                                //
// Mapping methods                                                //
//                                                                //
////////////////////////////////////////////////////////////////////

	// inherited from the base class

	// friend CondParamLvalue<Map,Word,Generator>::operator Word ( ) const;
	// friend Word& CondParamLvalue<Map,Word,Generator>::operator =( const Word& ) const;
	// Word imageOf( const Word& w ) const;
	// Word imageOf( const Generator& g ) const;
	// CondParamLvalue<Map,Word,Generator> imageOf( const Generator& g );
	// Word operator()( const Word& w ) const;
	// Word operator()( const Generator& g ) const;
	// CondParamLvalue<Map,Word,Generator> operator()( const Generator& g );
	// computing images:
	// Elt evalImage( const Word& w ) const;
	// Elt postEvalImage( const Word& w ) const;

// operations on maps:

	// Map& composeAfter( const Map& firstMap );
	// Map operator | ( const Map& secondEndo )

	Automorphism inverse() const;

////////////////////////////////////////////////////////////////////
//                                                                //
// Mapping properties                                             //
//                                                                //
////////////////////////////////////////////////////////////////////

	// inherited from the base class:

	// Trichotomy extendsToHom( ) const;
	// Trichotomy isMono( ) const;
	// Trichotomy isEpi( ) const;
	// Trichotomy isIso( ) const;
	// Trichotomy isTrivialMap( ) const; // @stc impl. tmp.

////////////////////////////////////////////////////////////////////
//                                                                //
// Debugging stuff                                                //
//                                                                //
////////////////////////////////////////////////////////////////////

#ifdef DEBUG

	//friend int main( );

	bool consistent( ) {
		const VectorOf<Word>& images = look()->theGeneratingImages;
		return Endomorphism::consistent() &&
		       ((const FreeGroup&)domain()).isAutomorphism(images);
	}

#endif
};

