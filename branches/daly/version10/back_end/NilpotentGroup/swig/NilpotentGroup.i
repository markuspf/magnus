%module Magnus

%{
#include "NilpotentGroup.h"
%}

class NilpotentGroup:public FGGroup{

public:

  //-------------------------------------------------------------------
  // 	Constructors / initializers
  //-------------------------------------------------------------------

  // To construct a group with named generators, given nilpotency class

  //FPNilpotentGroup
  NilpotentGroup(const VectorOf<Chars>& gennames, 
		 int nilclass,
		 const VectorOf<Word>& rels,
///		 enum NilpotentCollector::Type colType= NilpotentCollector::STANDARD);
		 NilpotentCollector::Type colType);

  //FreeNilpotentGroup
  %name(NilpotentGroupFromVector) NilpotentGroup(const VectorOf<Chars>& gennames, 
		 int nilclass,
//		 enum NilpotentCollector::Type collectorType 
//		                           = NilpotentCollector::STANDARD);
		 NilpotentCollector::Type colType);

  // To construct a group with unnamed generators, given nilpotency class

  //FPNilpotentGroup
  %name(NilpotentGroupFromIntVector)  NilpotentGroup(int numOfGen,
		 int nilclass,
		 const VectorOf<Word>& rels,
//		 enum NilpotentCollector::Type colType 
//		                          = NilpotentCollector::STANDARD);
		 NilpotentCollector::Type colType);

  //FreeNilpotentGroup
  %name(NilpotentGroupFromInt)  NilpotentGroup(int numOfGen,
		 int nilclass,
//		 enum NilpotentCollector::Type collectorType 
//		                          = NilpotentCollector::STANDARD);
		 NilpotentCollector::Type colType);


  void initialize() const;
  // Initializes the group.
  // FP initialization means computing of LCS factors.
  // Free NG is always initialized; nevertheless, you should always invoke
  // initialize(), for the group definition may be changed in the future.
  // pseudo-virtual

  //-------------------------------------------------------------------
  // 	Accessors
  //-------------------------------------------------------------------

  // Inherited from Group:
  Type type( );
  // dominates FGGroup::type();

  // Inherited from FGGroup:
  //  Type actualType() const; // pseudo-virtual
  //  int numberOfGenerators( ) const;
  //  Chars nameOfGenerator(int i) const;
  //  VectorOf<Chars> namesOfGenerators( ) const;

  bool isInitialized() const;
  // True iff the group is initialized
  // pseudo-virtual

  VectorOf<Word> relators() const;
  // Returns the group relators (empty vector if the group is free nilpotent)
  // pseudo-virtual

  VectorOf<Word> relationSubgroupGenerators() const;
  // Returns the generators of the relation subgroup
  // (empty vector if the group is free nilpotent)
  // pseudo-virtual

  VectorOf<PolyWord> basis() const;
  // The basis of the group, in terms of basic commutators (collected).
  // pseudo-virtual

  VectorOf<Chars> basisNames() const;
  // The names of the basis words
  // pseudo-virtual

  const BasicCommutators& commutators() const;
  //Returns the basic commutators of the group's free preimage

  int nilpotencyClass() const;
  // Returns the nilpotency class

  NGCollector collector() const;
  // Returns the current collector

  
  //-------------------------------------------------------------------
  // 	Methods dealing with the properties of the group:
  //-------------------------------------------------------------------

  // Inherited from FGGroup, redefined within Rep:
  //  int order( ) const;
  // Trichotomy isTrivial( ) const; // pseudo-virtual
  // Trichotomy isFinite( ) const; // pseudo-virtual
  // Trichotomy isInfinite( ) const; // pseudo-virtual
  // Trichotomy isAbelian( ) const; // pseudo-virtual

  virtual int theHirschNumber() const ;
  // returns the Hirsch number (the number of basic words
  // of infinite order)
  // pseudo-virtual
  
  
  // The following methods are meaningful only for FP nilpotent groups


  bool isFreeNilpotent( int* freeRank, int* freeClass ) const ;
  // returns true iff the group is free nilpotent or has free
  // nilpotent presentation.
  // in this case also returns its rank and nilpotency class.
  // pseudo-virtual

  int minimalNilpotencyClass( ) const ;
  // returns the minimal nilpotency class of the group
  // pseudo-virtual

  bool isMalcevBasis() const ;
  // returns truey iff the group basis is the Malcev basis
  // (has no words of finite order)
  // pseudo-virtual

  Integer orderOfTheTorsionSubgroup( ) const ;
  // returns order of the torsion subgroup
  // pseudo-virtual

  PresentationForNG makePresentation() const ;
  // produces FP presentation of the group

  VectorOf<Word> getLCSterm(int i) const ;
  // returns the generators of --th term of lower central series

  LCSQuotient getLCSQuotient( int N ) const ;
  // returns the N_th LCSQuotient 
  // can be used only after calling initialize()


  //-------------------------------------------------------------------
  //  Methods dealing with group elements (inherited):
  //-------------------------------------------------------------------

  // Inherited from FGGroup:
  // Elt makeIdentity( ) const;
  // Bool isSyntacticIdentity(const Elt& e) const;
  // Elt firstElt( ) const;
  // Elt nextElt(const Elt& e) const;
  // Elt multiply(const Elt& e1, const Elt& e2) const;
  // Elt inverseOf(const Elt& e) const;
  // Elt raiseToPower(const Elt& e, int n) const;
  // Elt conjugateBy(const Elt& e1, const Elt& e2) const;
  // Elt commutator(const Elt& e1, const Elt& e2) const;

  //@ep an Elt of this group is a Word, so I left these methods unchanged.
  // Most of them demand more careful design of the whole hierarchy.
  

  // The following methods are inherited and redefined within Rep

  // Elt eval( const Word& w ) const;
  // Trichotomy wordProblem( const Word& w ) const;
  // Trichotomy conjugacyProblem( const Word& u, const Word& v ) const;
  // Trichotomy isTrivialElt( const Elt& e ) const; // pseudo-virtual
  // Trichotomy areEqual(const Elt& e1, const Elt& e2) const;


  // Below are methods specific for this class

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with morpisms                 //
  //                                                   //
  ///////////////////////////////////////////////////////

  VectorOf<Word> inverseAuto(const VectorOf<Word>& V) const;
  // *** Time consuming algorithm ! ***
  // Returns the inverse  of automorphism, defined by V. 
  // Does not make check is it automorphism or not!!!

  //------------------------------------------------------------
  //  Conversions from one basis to another:
  //------------------------------------------------------------

  PolyWord decompose(const Word& w) const ;
  // decomposes the given word in terms of the group basis
  // input: word in terms of group generators
  // output: word in terms of group basis.
  // pseudo-virtual

  %name(decomposeFromPolyWord) PolyWord decompose(const PolyWord& w) const ;
  // decomposes the given word in terms of the group basis
  // input: word in terms of basic commutators (not collected)
  // output: word in terms of group basis.

  Word toWord(const PolyWord& pw) const ;
  //input: word in terms of the group basis
  //output: word in terms of the group generators
 

  PolyWord toCommutatorWord(const PolyWord& pw) const ;
  //input: word in terms of the group basis
  //output: word in terms of the basic commutators (not collected)

  Chars asDecomposition(const PolyWord& decomposition) const ;
  // Input: word in terms of group basis.
  // Output: text in terms of group basis.
  // pseudo-virtual


  //-----------------------------------------------------------
  //  Properties of the group elements:
  //-----------------------------------------------------------

  int weightOf(const Word& w) const ;
  // determines in which term of the lower central series w lies
  // nilClass + 1 if the word is trivial
  
  %name(weightOfFromPolyWord) int weightOf(const PolyWord& w) const ;
  // determines in which term of the lower central series w lies
  // nilClass + 1 if the word is trivial
  //w is given in terms of the group basis

  int orderOfBasic(Generator g) const ;
  //Returns order of the basic word corresponding to the generator g.
  //0 means infinity.

  int orderOf(const Word& w) const ;
  //Returns the order of the word given in terms of group generators

  %name(orderOfFromPolyWord) int orderOf(const PolyWord& w) const ;
  //Returns the order of the word given in terms of group basis
 
  VectorOf<Word> centralizer( const Word& w) const ;

  bool isInCommutatorSubgroup(const Word& w) const ;
  // returns true iff w is in the commutator subgroup, i.e. can be
  // represented as a product of commutators

  bool isCentral(const Word& w) const ;
  // returns true iff w lies in the group center
  // Slow, requires initialization

  void maximalRoot(const Word& w, PolyWord& root, int& power ) const ;
  // finds maximal root of pw.
  // w is given in terms of the group generators.
  // root is in terms of the group basis.

  %name(maximalRootFromPolyWord) void maximalRoot(const PolyWord& pw, PolyWord& root, int& power ) const ;
  // finds maximal root of pw.
  // pw is in terms of the group basis.
  // root is in terms of the group basis.
  // pseudo-virtual

  bool isProperPower(const Word& w) const ;
  // returns true iff the word is proper power
  // w is given in terms of the group generators.

  %name(isProperPowerFromPolyWord) bool isProperPower(const PolyWord& pw) const ;
  // returns true iff the word is proper power
  // pw is in terms of the group basis.

  
  //-------------------------------------------------------------------
  // I/O:
  //-------------------------------------------------------------------

  // Inherited from FGGroup:

  //  Word readWord(istream& istr, Chars& errMesg) const
  //  void printWord( ostream& ostr, const Word& w ) const
  //@ep temporarily disabled

  // Native:

//  void printBasis( ostream& BClist ) const ;
  // outputs the basis to a file
  // pseudo-virtual
  %addmethods{
    	char* printBasis() const {
		strstream *s=new strstream();
		self->printBasis(*s);
		(*s) << '\0' << flush;
		return s->str();
	}
  }

 
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Representation access methods                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  // operators inhereted from Group  

 

  //-------------------------------------------------------------------
  // Special methods
  //-------------------------------------------------------------------
/*
protected:

  // Special wrapping constructor to wrap new representations (returned
  // by eg. delegated methods) and for base initialisation by derived
  // classes:
 
  NilpotentGroup( NilpotentGroupRep* newrep ) 
    : DerivedObjectOf<FGGroup,NilpotentGroupRep>(newrep) { }
 
  static VectorOf<Chars> defaultNames(int num);
  // Produces vector of words "x1", "x2", ..... 
  // These are default names for group generators.

*/
};

