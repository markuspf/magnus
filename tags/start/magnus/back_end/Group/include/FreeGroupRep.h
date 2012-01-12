/*
 *   $Id: FreeGroupRep.h 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Definition of the FreeGroupRep class.
//
// Principal Authors: Stephane Collart, Roger Needham
//
// Status: in progress
//
// Revision History:
//

#ifndef _FREE_GROUP_REP_H_
#define _FREE_GROUP_REP_H_

#include "FGGroupRep.h"
#include "File.h"

struct FreeGroupRep : FGGroupRep {
  
public:
  
// Constructors:

  // Copy constructor and operator= provided by compiler (do deep copy).
  
  FreeGroupRep( int rank ) : FGGroupRep( rank ) { }
  
  FreeGroupRep( const VectorOf<Chars>& gennames ) :
    FGGroupRep( gennames ) { }
  
  // Destructor provided by compiler.

// Operators:

  private:
  FreeGroupRep& operator = ( const FreeGroupRep& fgr ); // disable assignment
  public:
  
// Representation methods:

  PureRep* clone( ) const { return new FreeGroupRep(*this); }
  // overrides PureRep* FGGroupRep::clone();

  static const Type theFreeGroupType;

  static Type type( ) { return theFreeGroupType; }
  // dominates FGGroup::type()

  Type actualType( ) const { return type(); }
  // overrides FGGroup::actualType()

// Group structure methods:

  int order( ) const;
  // overrides int FGGroupRep::order() const;
  Trichotomy isTrivial( ) const;
  // overrides int FGGroupRep::isTrivial() const;
  Trichotomy isFinite( ) const;
  // overrides int FGGroupRep::isFinite() const;
  Trichotomy isInfinite( ) const;
  // overrides int FGGroupRep::isInfinite() const;
  Trichotomy isAbelian( ) const;
  // overrides int FGGroupRep::isAbelian() const;

  bool compare( const GroupRep* G ) const;
  // overrides int FGGroupRep::compare() const;

  VectorOf<Word> nielsenBasis(const VectorOf<Word>& V) const; 
  // Returns a nielsen basis for the subgroup generated by `V'.

  Bool isAutomorphism(const VectorOf<Word>& V) const;
  // Returns TRUE iff generating vector `V' defines an 
  // automorphism of this group. The length of `V' should be
  // equal to the rank of this group.

  Bool isInnerAutomorphism(const VectorOf<Word>& V) const;
  // Returns TRUE iff generating vector `V' defines an inner
  // automorphism of this group. The length of `V' should be
  // equal to the rank of this group and it should define an
  // automorphism.

  bool isIAAutomorphism(const VectorOf<Word>& V) const;
  // Returns `true' iff generating vector `V' defines an 
  // IA-automorphism of this group. The length of `V' should be
  // equal to the rank of this group and it should define an
  // automorphism.

  VectorOf<Word> inverseAutomorphism(const VectorOf<Word>& V) const;
  // Returns a generating vector for the inverse automorphism.
  // `V' should define an automorphism of this group.

// Methods dealing with group elements:
  
  // Inhertited from FGGroupRep:
  // virtual Elt makeIdentity( ) const;
  // virtual Bool isSyntacticIdentity(const Elt&) const;

  Trichotomy isTrivialElt( const Elt& e ) const
	{ return Trichotomy( eval(e) == Word() ); }
  // overrides FGGroupRep::isTrivialElt()

  Elt eval( const Word& w ) const;
  // overrides
  Trichotomy areEqual(const Elt& e1, const Elt& e2) const;
  // overrides
  Trichotomy wordProblem( const Word& w ) const;
  // overrides
  Trichotomy conjugacyProblem( const Word& u, const Word& v ) const;
  // overrides

  Trichotomy conjugacyProblem( const Word& u, const Word& v ,Word& c) const;
  
  Bool inCommutatorSG(const Word& w) const;
  // Returns TRUE iff `w' is in the commutator subgroup.

  Bool isCommutator(const Word& w, Word& u1, Word& u2) const;
  // Returns TRUE iff `w' is a commutator. If TRUE, `u1' and `u2'
  // are set to words such that w = [u1,u2].

  Chars productOfCommutators( const Word& w , File& file );
  // If the word is in derived subgroup this procedure rewrites the 
  // word as a product of commutators.
  
  Chars productOfSquares( const Word& w , File& file );
  // If the word is in the subgroup generated by all squares this procedure 
  // rewrites it as a product of squares.
  
  Word getN_thElement( int n ) const;

  int numberOfElement( const Word& w ) const;
  
  // I/O:
  
  void printOn(ostream&) const;
  // overrides void FGGroupRep::printOn() const;
  GroupRep* readFrom(istream&, Chars&) const;
  // overrides FGGroupRep::


public:

  class NielsenBasis {
  public:
    NielsenBasis( const VectorOf<Word>& vec,
		  const VectorOf<Chars>& namesOfGenerators, 
		  bool writeToFile );
    // Copy ctor, operator= and dtor supplied by compiler.

    VectorOf<Word> oldGenerators( ) const { return theOldGenerators; }
    VectorOf<Word> newGenerators( );
    VectorOf<Word> expressNewGensInOldGens( );
    Chars getFileName( ) const; 
  private:
    void computeNielsenBasis( );
    void printParentGroup( ostream& ostr ) const;
    void printWord( ostream& ostr, const Word& w ) const;
    void printBasis( ostream& ostr, const VectorOf<Word>& basis, bool lineByLine ) const;
    void printTransformation( ostream& ostr, const VectorOf<Word>& basis, 
			      int i, int g1, int g2 ) const;

    // Data members:
    VectorOf<Chars> theNamesOfGenerators;
    VectorOf<Word> theOldGenerators;
    VectorOf<Word> theNewGenerators;
    VectorOf<Word> newGeneratorsExprssedInOldGerators;
    Chars theFileName;
    bool keepDetails;
    bool basisComputed;
  };

// No new data members.

private:
  
  // getN_thElement()'s accessories:

  Word getN_thWord( int n, int numberOfGenerators ) const;
  Generator numToGen( int n ) const;
  int genToNum( Generator g ) const;
  
};

#endif