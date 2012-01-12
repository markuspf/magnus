%module Magnus

%{
#include "APofFreeGroups.h"
#include "APofFreeGroupsRep.h"
%}

///From APofFreeGroupsRep.h

enum NumberOfFactor { LEFT_FACTOR = 0, RIGHT_FACTOR = 1, PRODUCT, INTERSECTION };
// This value indicates word membership. LEFT_FACTOR or RIGHT_FACTOR
// mean that the word consists entirely of generators of one factor,
// INTERSECTION means that the word belongs to the amalgamated subgroup
// and thus can be written in generators of any factor, and PRODUCT means
// the other cases.

//-------------------- LocalWord -------------------------------------

// A helper class: the word written in generators of one factor,
// together with the factor number.
//
// To understand the concept of local and global words, see comments in
// AmalgProductOfFreeGroups.h header.

struct LocalWord
{
  // methods

  LocalWord();

  %name(LocalWordFromWord) LocalWord(const Word& w, const NumberOfFactor& fac) ;

  // copy constructor, operator= and destructor supplied by compiler

//  operator Word() const;

//  friend LocalWord operator *;

//  LocalWord& operator *= (const LocalWord& w) ;

//  LocalWord inverse() const;

  LocalWord freelyReduce() const ;

  // data members

  Word theWord;
  NumberOfFactor theFactor;
   %addmethods{

	LocalWord __mul__(const LocalWord& w) {
		return (*self)*w;
	}

	LocalWord __invert__() {
		return self->inverse();
	}  
  }
};


///end  APofFreeGroupsRep.h

class AmalgProductOfFreeGroups:public FPGroup  {

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

  AmalgProductOfFreeGroups() ;
  // Default constructor is provided solely for constructing an AP group
  // from an input stream, e.g. like this:
  //
  //   AmalgProductOfFreeGroups AP;
  //   Chars errMesg = cin >> AP;
  //   ...
  // Do not use it for purposes other than this.

  %name(AmalgProductOfFreeGroupsFromFreeGroup) AmalgProductOfFreeGroups(const FreeGroup& g1, const FreeGroup& g2,
	                   const VectorOf<Word>& gen1,
			   const VectorOf<Word>& gen2  );
  // Construct an amalgamated product of given free groups. Given vectors
  // of words generate associated subgroups.

  %name(AmalgProductOfFreeGroupsFromSGofFreeGroup)  AmalgProductOfFreeGroups(const SGofFreeGroup& sg1, const SGofFreeGroup& sg2);
  // Construct an amalgamated product with given (associated) subgroups
  // which store references to their parent groups.

  %name(AmalgProductOfFreeGroupsFromGroup)  AmalgProductOfFreeGroups( const Group& G );
  // Cast construtor: to cast an arbitrary group as an AP group.
  // NB. This rewraps the unchanged representation, hence is in general
  // only useful for casting a group known to be an actual AP group.

  // Destructor provided by compiler.

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors / Modifiers                            //
  //                                                   //
  ///////////////////////////////////////////////////////

  Type type( );
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
	    
  Trichotomy isFree( ) const;
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

  VectorOf<Word> decompose(const Word& w) const;
  // Decomposes the given word to the product of words d_1 d_2 ....
  // such that every d_i belongs to one of the factors and any
  // two successive words belong to distinct factors.
  // If w is empty word, returns Vector of size 0.
    
  VectorOf<Word> reducedDecomposition(const Word& w) const;
  // Finds the minimal (in the sense of number of components) decomposition
  // of the given word. Each component belongs to one factor and any two 
  // successive components belong to different factors.
  // If w is identity element in AP, returns Vector of size 0.
  
  Word reducedFormOf(const Word& w) const ;
  // As above, but result is presented as a single word.
  
  VectorOf<Word> normalDecomposition(const Word& w) const;
  // Finds the normal decomposition of the given word: this is
  // the reduced decomposition where all components except the
  // first one are some right Schreier representatives.
  // Returns vector of components of decomposition.
  
  Word normalFormOf(const Word& w) const;
  // Finds the normal decomposition of the given word: this is
  // the reduced decomposition where all components except the
  // first one are some right Schreier representatives.
  // As above, but result is presented as a single word.

  void cyclicDecomposition(const Word& w, VectorOf<Word>& result, 
			   Word& conjugator) const ;
  // Finds cyclic reduced decomposition (named `result') of w such that 
  //          w = result^conjugator.
  // Cyclic decomposition is normal one satisfied the condition: all cyclic 
  // permutations are normal decompositions. Let a normal decomposition 
  // be g_1 * g_2 * ... * g_N, then the last one satisfies the condition above 
  // iff  N > 1 && factor(g1) != factor(gN)  or N <= 1.
  // Each word of `result' represents a component g_i of the decomposition.

  void cyclicReduction(const Word& w, Word& result, Word& conjugator) const;
  // Finds cyclic reduction (named `result') of w such that 
  //          w = result^conjugator.
  // Cyclic decomposition is normal one satisfied the condition: all cyclic 
  // permutations are normal decompositions. Let a normal decomposition 
  // be g1 * g2 * ... * gN, then the last one satisfies the condition above 
  // iff  N > 1 && factor(g1) != factor(gN)  or N <= 1.
  // As above, but `result' is presented as a single word.

  int numberOfSubstitutions( const Word& w ) const;
  // If the given word represents 1 in the group
  // the function computes the number of uses of a relation
  // a = b to deduce that w = 1, i.e. in re-expressing w as 
  // a product of conjugates of a * b^-1, computes the number
  // of these conjugates.

  
  NumberOfFactor factorOfFormalWord(const Word& w) const;
  // Determines the group the given formal word belongs to.
  
  NumberOfFactor factorOfElement(const Word& w) const ;
  // Determines the group the given formal word belongs to
  // and also checks whether the given element of the product belongs to 
  // the amalgamated subgroup.
  
  Word localToGlobal(const LocalWord& w) const;
  // Converts local coding word into global one.
  
  LocalWord globalToLocal(const Word& w) const ;
  // Converts global coding word into local one.
  
  
  Trichotomy isHyperbolic() const;
  // In general, the problem is undecidable, so this method can
  // return dontknow.
  //@ep In present, it _always_ returns dontknow -- temporary stub.
  
  
  void maximalRoot(const Word& w, Word& root, int& power) const ;
  // Finds maximal root of given word w. The root word is stored in `root',
  // and its power in `power'.
  //@ep  This is a temporary stub implemented only for one relator case.
  // Included here for maintaining inheritance. Invoking this causes
  // an error report.
  
  //@ep  The following two methods need maximalRoot to be implemented, so
  // they are now defined only for one relator case.
  
  bool isProperPower(const Word& w) const;
  // Determines whether w is a proper power.
  
  bool isProperPowerOfSecond(const Word& u, const Word& w, int& power) const ;
  // Determines whether u is a proper power of w.
  
  bool commute(const Word& u, const Word& w) const;
  // Determines whether [u,w] = 1.
  
  bool isSubgroupAbelian(const VectorOf<Word>& subgroupWords) const ;
  // Determines whether the subgroup generated by the given set of words
  // is abelian.

  bool isSubgroupTrivial(const VectorOf<Word>& vec) const ;
  // Determines whether subgroup generated by vec is trivial.

  bool isSubgroupCyclic(const VectorOf<Word>& vec) const ;
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

//  void printDecomposition(ostream& ostr, const VectorOf<Word> deco) const;
  %addmethods{
     	char* printDecomposition(const VectorOf<Word> deco) const {
		strstream *s=new strstream();
		self->printDecomposition(*s,deco);
		(*s) << '\0' << flush;
		return s->str();
	}
  }
  // Prints given vector of words which are separated by dot:
  //           word1 . word2 .     . wordN



///Inherited from FPGroup

  SetOf<Word> getRelators( ) const ;  

  // @stc code of the following incomplete: other flags must be reset
  // too:

  SetOf<Word> setRelators( const SetOf<Word>& r );

  SetOf<Word> addRelators( const SetOf<Word>& r ) ;

  SetOf<Word> removeRelators( const SetOf<Word>& r );

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods dealing with group structure             //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from FGGroup:
  //  int order( ) const;

  // Returns YES if this group is free on its generators, NO if not,
  // and DONTKNOW if no answer can be determined.

  Trichotomy isMetricSmallCancellationGroup( ) const;

  int cancellationLambda( ) const ;

  // @stc these methods for checking whether the group has small
  // cancellation are of dubious usefulness, since presumably in 
  // general the caller will want to construct symmetrised relations
  // to use in making a MSCGroup.

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from FGGroup:
  // Elt makeIdentity( ) const;
  // Bool isSyntacticIdentity(const Elt& e) const;
  // Trichotomy isTrivialElt( const Elt& e ) const; // pseudo-virtual
  // Trichotomy areEqual(const Elt& e1, const Elt& e2) const;
  // Elt firstElt( ) const;
  // Elt nextElt(const Elt& e) const;
  // Elt multiply(const Elt& e1, const Elt& e2) const;
  // Elt inverseOf(const Elt& e) const;
  // Elt raiseToPower(const Elt& e, int n) const;
  // Elt conjugateBy(const Elt& e1, const Elt& e2) const;
  // Elt commutator(const Elt& e1, const Elt& e2) const;
  // Elt eval( const Word& w ) const;
  // Trichotomy wordProblem( const Word& w ) const;
  // Trichotomy conjugacyProblem( const Word& u, const Word& v ) const;

  Word shortenByRelators(const Word& w) const ;
  // Shorten w by calling shortenByRelator with relators.
  // @stc @rn: this is outdated by @db & @dp's new implementation:
	  // This does not even symmetrize the relators, much less check a
	  // small cancellation condition or test hyperbolicity  -- that
	  // is the caller's responsibility.
  // @stc the question of whether to explicitely store symmetrised
  // relations or to provide fancy indexing is raised here too
  // (see Discussion, MSCGroup.h)

  Chars productOfCommutators(const Word& w,File& file);
  
  Chars productOfSquares(const Word& w,File& file);

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Fabricators:                                     //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from FGGroup:
  //  Subgroup randomSubgroup( ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  I/O                                              //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from FGGroup:

  //  Word readWord(istream& istr, Chars& errMesg) const
  //  void printWord( ostream& ostr, const Word& w ) const

  

 /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  // operators inhereted from Group

  %addmethods {
  ///void printRelators(ostream& ostr) const ;
	char* printRelators() const {
		strstream *s=new strstream();
		self->printRelators(*s);
		(*s) << '\0' << flush;
		return s->str();
	}

///  friend ostream& operator << ( ostream&, const Group& );
          char* __str__() {
		strstream s;
		s << *self << '\0';
		return s.str();
          }
//  int operator == ( const Group& G ) const 

  // @rn:stc Can we do better than this?
  // @dp made some enhancements.
  
 // int operator != ( const Group& G ) const { return !(*this == G); }
           int __cmp__(const Group& w) {
         	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
           }
//inherited from FGGroup
//  void printGenerator( ostream& ostr, const Generator& g ) const;
  // Outputs the print name of g on ostr, followed by "^-1" if ord(g) < 0.
	char* printGenerator(const Generator& g) const {
		strstream *s=new strstream();
		self->printGenerator(*s,g);
		(*s) << '\0' << flush;
		return s->str();
	}


//  void printGenerators( ostream& ostr ) const;
  // Calls printGenerator for each generator of this group, separating
  // them with commas.
	char* printGenerators() const {
		strstream *s=new strstream();
		self->printGenerators(*s);
		(*s) << '\0' << flush;
		return s->str();
	}
//  void printWord( ostream& ostr, const Word& w ) const;
  // Outputs w to ostr, in a form that can input by readWord.
	char* printWord(const Word& w) const {
		strstream *s=new strstream();
		self->printWord(*s,w);
		(*s) << '\0' << flush;
		return s->str();
	}
//  void printSetOfWords( ostream& ostr, const SetOf<Word>& S ) const;
  // Outputs S to ostr, in a form that can input by readWord.
	char* printSetOfWords(const SetOf<Word>& S) const {
		strstream *s=new strstream();
		self->printSetOfWords(*s,S);
		(*s) << '\0' << flush;
		return s->str();
	}
//  void printVectorOfWords( ostream& ostr, const VectorOf<Word>& V,
//	    char* leftBracket = "{", char* rightBracket = "}" ) const;
  // Outputs V to ostr, in a form that can input by readWord.
	char* printVectorOfWords(const VectorOf<Word>& V,
	    char* leftBracket = "{", char* rightBracket = "}" ) const {
		strstream *s=new strstream();
		self->printVectorOfWords(*s,V,leftBracket);
		(*s) << '\0' << flush;
		return s->str();
	}
  }

///inherited from FGGroup
  int numberOfGenerators( ) const;
  
  Chars nameOfGenerator(int i) const;
  // Returns print name of (i + 1)th generator. Zero-based indexing.
  // No negative i's; does not return print names of formal inverses.
  // @rn Take a Generator instead?

  %name(namesOfGeneratorsFromGenerator) Chars nameOfGenerator(Generator g) const; 

  %name(namesOfGeneratorsNoParam) VectorOf<Chars> namesOfGenerators( ) const;
  // Returns vector of print names of all generators, in the order of
  // their ordinals.

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Group structure methods                          //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from Group:

   Trichotomy isTrivial( ) const; // pseudo-virtual
   Trichotomy isFinite( ) const; // pseudo-virtual
   Trichotomy isInfinite( ) const; // pseudo-virtual
   Trichotomy isAbelian( ) const; // pseudo-virtual

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////

  
  Elt eval( const Word& w ) const;
  // Takes a word and evaluates it as a formal word in the generators
  // of the group. Returns a (possibly canonic) representative of the
  // element. This method is pseudo-virtual.

  Trichotomy wordProblem( const Word& w ) const;
  // Returns YES if w represents the identity, NO if not, and
  // DONTKNOW if no answer can be determined.

  Trichotomy conjugacyProblem( const Word& u, const Word& v ) const;
  // Returns YES if u and v represent conjugate elements, NO if not, and
  // DONTKNOW if no answer can be determined.

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with sets of group elements   //
  //                                                   //
  ///////////////////////////////////////////////////////


  void closeUnderCyclicPermutations(SetOf<Word>& S) const;
  // Alters S so that if w is in S, then any cyclic permutation of w is too.

  ///////////////////////////////////////////////////////
  //                                                   //
  //  I/O                                              //
  //                                                   //
  ///////////////////////////////////////////////////////

  Word readWord(istream& istr, Chars& errMesg) const;
  // Tries to parse and return a Word from istr. At present, the word
  // in istr must end in ',', so the parser knows when to stop.
  // If there is a parse error, a message is returned in errMesg,
  // and the empty Word is returned.

  SetOf<Word> readSetOfWords(istream& istr, Chars& errMesg) const;
  // Tries to parse and return a set of Words, delimited by '{', '}' and
  // comma-separated, from istr.
  // If there is a parse error, a message is returned in errMesg,
  // and the empty set is returned.

  VectorOf<Word> readVectorOfWords(istream& istr, Chars& errMesg) const;
  // Tries to parse and return a set of Words, delimited by '{', '}' and
  // comma-separated, from istr.
  // If there is a parse error, a message is returned in errMesg,
  // and the empty set is returned.






/*
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
*/
};


