// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Definition of the AmalgProductOfFreeGroups class.
//
// Principal Authors: Eugene Paderin, Dmitry Pechkin
//
// Status: in progress
//
// Revision History:
//
//   02-10-96: the preliminary release.
//
//   03-29-96: bugfix, more comments added.
//
// Special Notes:
//
//  * Local And Global Words
//
//  When dealing with AP words, we use two distinct ways of coding
//  generators. The AP is defined with two groups which of them has
//  its own "local" coding of generators. The result is an FP group
//  generated by the same generators, but coded the different way
//  (generators of the first group have the same numbers while those
//  of the second one have shifted numbers -- we call this coding
//  global). The user gives words in global coding using letters
//  from both groups -- we do not redefine FPGroup::readWord and
//  FPGroup::writeWord (so the FrontEnd developer should take this
//  into account). At the same time, all manipulations with subgroups
//  and their bases are being done in local coding, so we have to
//  convert words from one coding into another, and vice versa.
//
//

#ifndef _AMALGAMATED_PRODUCT_H_
#define _AMALGAMATED_PRODUCT_H_

#include "FreeGroup.h"
#include "FPGroup.h"
#include "APofFreeGroupsRep.h"


class AmalgProductOfFreeGroups : public DerivedObjectOf<FPGroup,AmalgProductOfFreeGroupsRep> {

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors                                     //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Copy constructor, operator= and destructor provided by compiler.

  // To define the amalgamated product (AP), one should give two
  // (free) groups as factors, and two vectors of Words which define
  // two subgroups in corresponding groups (so the vectors must be bases).
  // Then we claim the subgroups to be amalgamated, i.e. we suppose
  // that the ith word of the first vector is mapped to the ith word
  // of the second one. This is a standard way to define an AP.

  // At the same time, we define the AP as a FP group, so all the
  // methods of FPGroup is fully applicable to the AP class.

  AmalgProductOfFreeGroups() :
  DerivedObjectOf<FPGroup, AmalgProductOfFreeGroupsRep>(
	new AmalgProductOfFreeGroupsRep(FreeGroup(), FreeGroup(),
					VectorOf<Word>(), VectorOf<Word>() )
    ) {}
  // Default constructor is provided solely for constructing an AP group
  // from an input stream, e.g. like this:
  //
  //   AmalgProductOfFreeGroups AP;
  //   Chars errMesg = cin >> AP;
  //   ...
  // Do not use it for purposes other than this.

  AmalgProductOfFreeGroups(const FreeGroup& g1, const FreeGroup& g2,
	                   const VectorOf<Word>& gen1,
			   const VectorOf<Word>& gen2  ) :
    DerivedObjectOf<FPGroup, AmalgProductOfFreeGroupsRep>(
    	new AmalgProductOfFreeGroupsRep(g1, g2, gen1, gen2)
    ) {}
  // Construct an amalgamated product of given free groups. Given vectors
  // of words generate associated subgroups.

  AmalgProductOfFreeGroups(const SGofFreeGroup& sg1, const SGofFreeGroup& sg2)
    : DerivedObjectOf<FPGroup, AmalgProductOfFreeGroupsRep>(
        new AmalgProductOfFreeGroupsRep(sg1, sg2) 
      ) {}
  // Construct an amalgamated product with given (associated) subgroups
  // which store references to their parent groups.

  AmalgProductOfFreeGroups( const Group& G ) :
    DerivedObjectOf<FPGroup,AmalgProductOfFreeGroupsRep>(G) { }
  // Cast construtor: to cast an arbitrary group as an AP group.
  // NB. This rewraps the unchanged representation, hence is in general
  // only useful for casting a group known to be an actual AP group.

  // Destructor provided by compiler.

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors / Modifiers                            //
  //                                                   //
  ///////////////////////////////////////////////////////

  static Type type( ) { return AmalgProductOfFreeGroupsRep::type(); }
  // Overrides FPGroup::type();
  
  // Type actualType() const; 
  // Overrides pseudo-virtual FPGroup::actualType().


  FreeGroup factor( const NumberOfFactor& t ) const;
  // Returns left or right factor of the amalgamated product.
  // LEFT_FACTOR and RIGHT_FACTOR values are valid for number of factor.

  SGofFreeGroup subgroup( const NumberOfFactor& t ) const;
  // Returns amalgamated subgroup of left or right factor respectively.
  // LEFT_FACTOR and RIGHT_FACTOR values are valid for number of factor.

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods dealing with group structure             //
  //                                                   //
  ///////////////////////////////////////////////////////

  // int order( ) const;   
  // Returns the order of the AP as the FP group.
  // Overrides pseudo-virtual FPGroup::order().
	    
  Trichotomy isFree( ) const { return enhance()->isFree(); }
  // Returns YES if this group is free on its generators, NO if not,
  // and DONTKNOW if this cannot be currently determined.
  //@ep  Now gives answer only for some special cases;
  //     to be enhanced in the future.
  
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////
      
  // Elt eval( const Word& w ) const;
  // Takes a word and evaluates it as a formal word in the generators
  // of the group. Returns a (possibly canonic) representative of the
  // element. 
  // Overrides pseudo-virtual FPGroup::eval().

  // Trichotomy wordProblem( const Word& w ) const;
  // Returns YES if w represents the identity, NO if not, and
  // DONTKNOW if no answer can be determined.
  // Overrides pseudo-virtual FPGroup::wordProblem().

  VectorOf<Word> decompose(const Word& w) const {
    return look()->decompose(w);
  }
  // Decomposes the given word to the product of words d_1 d_2 ....
  // such that every d_i belongs to one of the factors and any
  // two successive words belong to distinct factors.
  // If w is empty word, returns Vector of size 0.
    
  VectorOf<Word> reducedDecomposition(const Word& w) const {
    return look()->reducedDecomposition(w);
  }
  // Finds the minimal (in the sense of number of components) decomposition
  // of the given word. Each component belongs to one factor and any two 
  // successive components belong to different factors.
  // If w is identity element in AP, returns Vector of size 0.
  
  Word reducedFormOf(const Word& w) const {
    return compose(reducedDecomposition(w));
  }
  // As above, but result is presented as a single word.
  
  VectorOf<Word> normalDecomposition(const Word& w) const {
    return look()->normalDecomposition(w);
  }
  // Finds the normal decomposition of the given word: this is
  // the reduced decomposition where all components except the
  // first one are some right Schreier representatives.
  // Returns vector of components of decomposition.
  
  Word normalFormOf(const Word& w) const {
    return compose(normalDecomposition(w));
  }
  // Finds the normal decomposition of the given word: this is
  // the reduced decomposition where all components except the
  // first one are some right Schreier representatives.
  // As above, but result is presented as a single word.

  void cyclicDecomposition(const Word& w, VectorOf<Word>& result, 
			   Word& conjugator) const 
  {
    look()->cyclicDecomposition(w,result,conjugator);
  }
  // Finds cyclic reduced decomposition (named `result') of w such that 
  //          w = result^conjugator.
  // Cyclic decomposition is normal one satisfied the condition: all cyclic 
  // permutations are normal decompositions. Let a normal decomposition 
  // be g_1 * g_2 * ... * g_N, then the last one satisfies the condition above 
  // iff  N > 1 && factor(g1) != factor(gN)  or N <= 1.
  // Each word of `result' represents a component g_i of the decomposition.

  void cyclicReduction(const Word& w, Word& result, Word& conjugator) const {
    look()->cyclicReduction(w,result,conjugator);
  }
  // Finds cyclic reduction (named `result') of w such that 
  //          w = result^conjugator.
  // Cyclic decomposition is normal one satisfied the condition: all cyclic 
  // permutations are normal decompositions. Let a normal decomposition 
  // be g1 * g2 * ... * gN, then the last one satisfies the condition above 
  // iff  N > 1 && factor(g1) != factor(gN)  or N <= 1.
  // As above, but `result' is presented as a single word.

  int numberOfSubstitutions( const Word& w ) const {
    return look()->numberOfSubstitutions( w );
  }
  // If the given word represents 1 in the group
  // the function computes the number of uses of a relation
  // a = b to deduce that w = 1, i.e. in re-expressing w as 
  // a product of conjugates of a * b^-1, computes the number
  // of these conjugates.

  
  NumberOfFactor factorOfFormalWord(const Word& w) const {
    return look()->factorOfFormalWord(w);
  }
  // Determines the group the given formal word belongs to.
  
  NumberOfFactor factorOfElement(const Word& w) const {
    return look()->factorOfElement(w);
  }
  // Determines the group the given formal word belongs to
  // and also checks whether the given element of the product belongs to 
  // the amalgamated subgroup.
  
  Word localToGlobal(const LocalWord& w) const {
    return look()->localToGlobal(w);
  }
  // Converts local coding word into global one.
  
  LocalWord globalToLocal(const Word& w) const {
    return look()->globalToLocal(w);
  }
  // Converts global coding word into local one.
  
  
  Trichotomy isHyperbolic() const {
    return look()->isHyperbolic();
  }
  // In general, the problem is undecidable, so this method can
  // return dontknow.
  //@ep In present, it _always_ returns dontknow -- temporary stub.
  
  
  void maximalRoot(const Word& w, Word& root, int& power) const {
    look()->maximalRoot(w,root,power);
  }
  // Finds maximal root of given word w. The root word is stored in `root',
  // and its power in `power'.
  //@ep  This is a temporary stub implemented only for one relator case.
  // Included here for maintaining inheritance. Invoking this causes
  // an error report.
  
  //@ep  The following two methods need maximalRoot to be implemented, so
  // they are now defined only for one relator case.
  
  bool isProperPower(const Word& w) const {
    return look()->isProperPower(w);
  }
  // Determines whether w is a proper power.
  
  bool isProperPowerOfSecond(const Word& u, const Word& w, int& power) const {
    return look()->isProperPowerOfSecond( u, w, power );
  }
  // Determines whether u is a proper power of w.
  
  bool commute(const Word& u, const Word& w) const {
    return look()->commute(u,w);
  }
  // Determines whether [u,w] = 1.
  
  bool isSubgroupAbelian(const VectorOf<Word>& subgroupWords) const {
    return look()->isSubgroupAbelian(subgroupWords);
  }
  // Determines whether the subgroup generated by the given set of words
  // is abelian.

  bool isSubgroupTrivial(const VectorOf<Word>& vec) const {
    return look()->isSubgroupTrivial(vec);
  }
  // Determines whether subgroup generated by vec is trivial.

  bool isSubgroupCyclic(const VectorOf<Word>& vec) const {
    return look()->isSubgroupCyclic(vec);
  }
  // Determines whether subgroup generated by vec is cyclic.
  //@ep Invokes maximalRoot, so it is unusable now.


  ///////////////////////////////////////////////////////
  //                                                   //
  //  I/O                                              //
  //                                                   //
  ///////////////////////////////////////////////////////

  // void printRelators(ostream& ostr) const;
  // Overrides pseudo-virtual FPGroup::printRelators().

  // void printOn(ostream& o) const;
  // Overrides pseudo-virtual FPGroup::printOn().

  // GroupRep* readFrom(istream& istr, Chars& errMesg) const;
  // Overrides pseudo-virtual FPGroup::readFrom().

  void printDecomposition(ostream& ostr, const VectorOf<Word> deco) const {
    look()->printDecomposition(ostr, deco);
  }
  // Prints given vector of words which are separated by dot:
  //           word1 . word2 .     . wordN

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Private helper stuff:                            //
  //                                                   //
  ///////////////////////////////////////////////////////

protected:

  // Special wrapping constructor to wrap new representations (returned
  // by eg. delegated methods) and for base initialisation by derived
  // classes:
  
  AmalgProductOfFreeGroups( AmalgProductOfFreeGroupsRep* newrep ) :
  DerivedObjectOf<FPGroup,AmalgProductOfFreeGroupsRep>(newrep) { }
  

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Debugging stuff                                  //
  //                                                   //
  ///////////////////////////////////////////////////////

private:

#ifdef DEBUG
  //friend int main( );
  friend void debugPrint(ostream&, const AmalgProductOfFreeGroups& g);
#endif

};

#endif


