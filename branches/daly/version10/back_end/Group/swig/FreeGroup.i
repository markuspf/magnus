/*

FreeGroup.i
$Id$

This one is pretty straightforward. The list of methods that get wrapped
may have to be rethought at some point. Note that inherited functions have
to be wrapped here if they are supposed to be available to FreeGroup.

Peter Brinkmann, 05/18/2001
brinkman@math.uiuc.edu
*/

%module Magnus

%{
#include "FreeGroup.h"
%}

// class Aut; // Doesn't exist yet

class FreeGroup:public FGGroup {
  
public:
  
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors                                     //
  //                                                   //
  ///////////////////////////////////////////////////////
  
  // Copy constructor provided by compiler


  FreeGroup( int rank );
  // To construct a group of given rank.
  // Default constructor gives trivial group with rank 0.
  
%name(FreeGroupFromList) FreeGroup( const VectorOf<Chars>& gennames );
//    DerivedObjectOf<FGGroup,FreeGroupRep>( new FreeGroupRep(gennames) ) { }
  // To construct a group with named generators.
  
  // Destructor provided by compiler.
  
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Operators                                        //
  //                                                   //
  ///////////////////////////////////////////////////////

  // assignement operator provided by compiler
  
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors                                        //
  //                                                   //
  ///////////////////////////////////////////////////////

  Type type( );
  // dominates FGGroup::type();

  // Inherited from FGGroup: @rn update
  Type actualType() const; // pseudo-virtual
  int numberOfGenerators( ) const;
  Chars nameOfGenerator(int i) const;
  VectorOf<Chars> namesOfGenerators( ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Group structure methods                          //
  //                                                   //
  ///////////////////////////////////////////////////////

  int rank( ) const;
  
  // members to manipulate homs :
  
  // Aut randomAut( ) const; // @stc some thought has to be put into this and
                          // into manipulating distributions

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods dealing with group elements              //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from FGGroup: @rn update
  // Elt makeIdentity( ) const; // pseudo-virtual
  // Bool isSyntacticIdentity(const Elt& e) const; // pseudo-virtual
  // Trichotomy isTrivialElt( const Elt& e ) const; // pseudo-virtual
  // Trichotomy areEqual(const Elt& e1, const Elt& e2) const; // pseudo-virtual
  // Elt firstElt( ) const; // pseudo-virtual
  // Elt nextElt(const Elt& e) const; // pseudo-virtual
  // Elt multiply(const Elt& e1, const Elt& e2) const; // pseudo-virtual
  // Elt inverseOf(const Elt& e) const; // pseudo-virtual
  // Elt raiseToPower(const Elt& e, int n) const; // pseudo-virtual
  // Elt conjugateBy(const Elt& e1, const Elt& e2) const; // pseudo-virtual
  // Elt commutator(const Elt& e1, const Elt& e2) const; // pseudo-virtual
  // Elt eval( const Word& w ) const;
  Trichotomy wordProblem( const Word& w ) const;
  // Trichotomy conjugacyProblem( const Word& u, const Word& v ) const;

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
  // rewrites it as a product of commutators.

  Word getN_thElement( int n ) const;
  // Get the n-th element of a free group.

  int numberOfElement( const Word& w ) const;
  // Computes the number of the word in the free group.

  VectorOf<Word> nielsenBasis(const VectorOf<Word>& V) const;
  // Returns a nielsen basis for the subgroup generated by `V'.

  // NielsenBasis nielsenBasis(const VectorOf<Word>& V, bool writeToFile ) const;
  // Returns a nielsen basis for the subgroup generated by `V'
  // and decomposition of the nielsen basis in V.

  Bool isAutomorphism(const VectorOf<Word>& V) const;
  // Returns TRUE iff generating vector `V' defines an
  // automorphism of this group. The length of `V' should be
  // equal to the rank of this group.

  Bool isInnerAutomorphism(const VectorOf<Word>& V) const;
  // Returns TRUE iff generating vector `V' defines an inner
  // automorphism of this group. The length of `V' should be
  // equal to the rank of this group and `V' should define an
  // automorphism.

  bool isIAAutomorphism(const VectorOf<Word>& V) const;
  // Returns `true' iff generating vector `V' defines an
  // IA-automorphism of this group. The length of `V' should be
  // equal to the rank of this group and `V' should define an
  // automorphism.

  VectorOf<Word> inverseAutomorphism(const VectorOf<Word>& V) const;
  // Returns a generating vector for the inverse automorphism.
  // `V' should define an automorphism of this group.

//  Map inverseAutomorphism( const Map& M) const;
  // Returns an inverse automorphism.

  
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods dealing with subgroups                   //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from FGGroup: @rn update
  //  Subgroup randomSubgroup( )

  ///////////////////////////////////////////////////////
  //                                                   //
  //  I/O                                              //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from FGGroup: @rn update
  Word readWord(istream& istr, Chars& errMesg) const;
  // void printGenerators( ostream& ostr ) const;
  // void printWord( ostream& ostr, const Word& w ) const
 
%addmethods{

	char* printWord(const Word& w) const {
		strstream *s=new strstream();
		self->printWord(*s,w);
		(*s) << '\0' << flush;
		return s->str();
	}

	char* __str__() {
		strstream s;
		s << *self << '\0';
		return s.str();
	}

	Chars firstGenerator() {
		return self->namesOfGenerators()[0];
	}
}

};

%inline %{
	FreeGroup* FreeGroupFromString(char* p) {
		strstream s;
		FreeGroup* F=new FreeGroup();
		s << p << '\0' << flush;
		Chars msg=s >> *F;
		if (msg.length()>0) {
			PyErr_SetString(PyExc_Exception,msg);
			PyErr_Print();
			return NULL;
		}
		return F;
	}
%}

