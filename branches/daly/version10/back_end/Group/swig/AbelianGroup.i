%module Magnus

%{
#include "AbelianGroup.h"
//#include "FPGroup.h"
//#include "AbelianGroupRep.h"
#include "AbelianSGPresentation.h"
%}

class AbelianSGPresentation;

class AbelianGroup 
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Constructors:                                                      //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  AbelianGroup( const FPGroup& G, bool makeFile = false );
  // Constructs an abelian group with presentation G.
  // If bMakeFile is true, expression of new generators in terms
  // of old ones will be written in a file after the computation
  // is finished.
  
  // No default constructor, but it can be easily arranged by
  // combining default FPGroup constructor and previsious one.

  // Copy constructor and operator = provided by compiler
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Accessors:                                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
 
  void computeCyclicDecomposition( ) ;
  // *** Time consuming algorithm ! ***
  // Computes cyclic decomposition of the abelian group.

  void findPrimaryBasis() ;
  // *** Time consuming algorithm ! ***
  // Computes primary decomposition of the abelian group.  

  bool haveCyclicDecomposition( ) const ;
  // True if the cyclic decomposition has been computed.

  bool havePrimaryDecomposition( ) const ;
  // True if the primary decomposition has been computed.
  
  Chars getFileName( ) const ;
  // Returns name of the file containing new generators.
  // Calls error() if no file has been created.

  Chars getFileNameOfPDGens( ) const;
  // Returns name of the file containing generators for prime decomposition.
  // Calls error() if no file has been created.

  FPGroup getFPGroup( ) const ;
  // Returns the original group presentation.


  SetOf<Word> getAllRelators( ) const ;
  // Returns all relators of the original group plus all commutators.


  AbelianWord oldInAbelianForm( const Word& w ) const ;
  // Computes the abelian form of a word written in terms of 
  // the old generators.
  

  /////////////////////////////////////////////////////////////////////////
  //  
  //  The rest of the public functions in the class can be used only if
  //  the cyclic decomposition is computed.
  //
  //
  
  int rankOfFreeAbelianFactor( ) const;
  // Returns rank of the free part.

  
  VectorOf<Integer> invariants( ) const ;
  // Return the invariants m_i such that m_i | m_{i+1}. All m_i > 1.

  
  VectorOf<AbelianWord> oldToNewGens() const ;
  // Returns images of the old generators in terms of the
  // new set of generators.
  
  VectorOf<AbelianWord> newToOldGens() const ;
  // Returns images of the new generators in terms of the
  // new set of generators.
  
  AbelianGroup getCanonicalSmithPresentation( ) const;
  // Returns the cyclic decomposition as an object of the class AbelianGroup.
  // In that object the new generators and the old generators are the same;
  // the cyclic decomposition, invariants, the rank of free part and
  // transformation vectors are already computed.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods and operators which deal with the group:                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  Integer order() const ;
  // Returns order of the group, 0 means infinity.


  bool isTrivial() const ;
  // Returns true if the group is trivial.


  bool isFinite() const;
  // Returns true if the group is finite.

  
  bool isInfinite() const ;
  // Returns true if the group is infinite.

  
  bool isFree() const ;
  // Returns true if the group is free.

  
  bool isomorphicTo( const AbelianGroup& G) const ;
  // Returns true if the group is isomorphic to G.


  AbelianGroup computeIntegralHomology( int n ) const;
  // Returns the H_n group of this one.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods and operators which deal with subgroups:                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Integer orderOfTheTorsionSubgroup( ) const ;
  // Returns order of the torsion subgroup.

  AbelianSGPresentation makeSubgroupPresentation(const VectorOf<Word>& vG) const;

  // Makes presentation of subgroup 

  VectorOf<Word> findSubgroupIsolator(const VectorOf<Word>& vG) const;
  // *** Time consuming algorithm ! ***
  // Returns the generators of Isolator of given subgroup

  VectorOf<Word> findVirtFreeComplementOfSG(const VectorOf<Word>& vG) const;
  // *** Time consuming algorithm ! ***
  // returns generators of virtual free complement, of a given subgroup

  VectorOf<Word> joinSubgroups(const VectorOf<Word>& vG1,const VectorOf<Word>& vG2) const;
  // *** Time consuming algorithm ! ***
  // Returns the generators of join subgroup of given groups
  
  VectorOf<Word> findSubgIntersection( const VectorOf<Word>& vG1 ,
				       const VectorOf<Word>& vG2 , 
				       File& file ) const;
  // Returns the generators for intersection of given subgroups  
  
  bool isPureCyclSubgroup(const Word& w) const;
  // True if given cyclic subgroup is pure
 
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods which deal with group elements:                            //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool areEqual(const Word& u, const Word& v) const;
  // Returns true if u and v are equal.
  

  %name(isTrivialFromWord) bool isTrivial( const Word& w ) const;
  // Word problem.

  
  Integer orderOfElt( const Word& w ) const;
  // Returns the order of an element, 0 means infinity.


  %name(newToOldGensFromAbelianWord) AbelianWord newToOldGens( const AbelianWord& w ) const;
  // Converts an element from the new generators to the old ones.

  %name(oldToNewGensFromAbelianWord) AbelianWord oldToNewGens( const AbelianWord& w ) const;
  // Converts an element from the old generators to the new ones.

  AbelianWord findEltPrimeForm(const Word& w) const;
  // Converts an element from the old generators to the prime form ones.


  AbelianWord pBlockOfElt( const AbelianWord& w,Integer p )const;
  // Returns a p-block of an element. 
  
  %name(pBlockOfEltFromWord) AbelianWord pBlockOfElt( const Word& w,Integer p )const;
  // Returns a p-block of an element. 
  
  
      
//  Integer pHeightOfElt(const Word& w, const Integer& p, Integer orderofElt = -1);
  Integer pHeightOfElt(const Word& w, const Integer& p, Integer orderofElt);
  // *** Time consuming algorithm ! ***
  // Returns a p-height of a given alement. The orderofElt - is an order of
 
  Integer powerOfEltInSubgroup(const Word& w,const VectorOf<Word>& sGroup) const;
  // Returns the power of element in subgroup

  bool isEltProperPower(const Word& w) const;
  // *** Time consuming algorithm ! ***
  // True if element is proper power

  void abelianMaximalRoot(const Word& w, Word& maxRoot, Integer& maxExp) const;
  // *** Time consuming algorithm ! ***
  // Compute the maximal root of an element. Put maximal root in  maxRoot and
  // maximal exponent in maxExp. If maxExp is 0, than there is no maximal root fpr
  // this element

  AbelianWord primeFormInOldGens(const AbelianWord& w) const;
  // Converts an element in prime form  to the old generators.
 
  int isPowerOfSecond(const Word& word1, const Word& word2) const;
  // *** Time consuming algorithm ! ***
  // If word1 is a power of word2, returns the power, if not returns 0
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods which deal with morphisms:                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Bool isEpimorphism(const VectorOf<Word>& V) const;
  // *** Time consuming algorithm ! ***
  // Returns TRUE iff generating vector `V' defines an 
  // epimorphism of this group. The length of `V' should be
  // equal to the number of generators of this group. 

  int orderOfAuto(const VectorOf<Word>& V) const;
  // *** Time consuming algorithm ! ***
  // Returns the order of Automorphism defined by V. 
  // Does not make check is it automorphism or not!!!
  // Returns -1 if infinite (temporary, if order > 1000)

  VectorOf<Word> inverseAuto(const VectorOf<Word>& V) const;
  // *** Time consuming algorithm ! ***
  // Returns the inverse  of automorphism, defined by V. 
  // Does not make check is it automorphism or not!!!

  VectorOf<Word> fixedPointsOfAuto( const VectorOf<Word>& v ) const;
  // Returns the generating set for the subgroup of fixed points of the 
  // automorphism defined by v.
  // Does not make check is it automorphism or not!!!
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O:                                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


  // Print the cyclic decomposition in the form, like Z^5 x Z_2 x Z_4^2 ...

  
  Integer test(Integer orderofElt);
  // Print the prymary decomposition in the form, like Z^5 x Z_2 x Z_4 x Z_3 ...

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////




/*  friend ostream& operator < ( ostream& ostr, const AbelianGroup& G )
    friend istream& operator > ( istream& istr, AbelianGroup& G )
*/  

  %addmethods {
      //  friend ostream& operator << ( ostream& ostr, const AbelianGroup& G );

	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}

//  void printWordInNewGens( ostream& ostr, const AbelianWord& w) const;
  // Print a word in the new generators in additive notation.
	char* printWordInNewGens(const AbelianWord& w) const {
		strstream *s=new strstream();
		self->printWordInNewGens(*s,w);
		(*s) << '\0' << flush;
		return s->str();
	}
//  void printInPrimaryForm(ostream& ostr, const AbelianWord& aw) const;
  // Print a word in primary form: f1 + 2 p2 ... (aw must be in prime form)
	char* printInPrimaryForm(const AbelianWord& w) const {
		strstream *s=new strstream();
		self->printInPrimaryForm(*s,w);
		(*s) << '\0' << flush;
		return s->str();
	}
//  void printPrimaryDec( ostream& ostr) const;
	char* printPrimaryDec() const {
		strstream *s=new strstream();
		self->printPrimaryDec(*s);
		(*s) << '\0' << flush;
		return s->str();
	}


  }
};
