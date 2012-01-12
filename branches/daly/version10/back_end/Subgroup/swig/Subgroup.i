%module Magnus

%{
#include "Subgroup.h"

%}

class Subgroup : public FGGroup {

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors                                     //
  //                                                   //
  ///////////////////////////////////////////////////////

  // @stc at some point, this default construtor may be wanted (see notes)
  // Subgroup( ) : FGGroup( new SubgroupRep() ) { }
  // to initialise an unspecified subgroup

  Subgroup( const FGGroup& parent );
  // to make the trivial subgroup

  %name(SubgroupFromVector) Subgroup( const FGGroup& parent, const VectorOf<Word>& gens );
  // to make a finitely generated subgroup with a vector of unnamed
  // generators

  %name(SubgroupFromSet) Subgroup( const FGGroup& parent, const SetOf<Word>& gens );
  // to make a finitely generated subgroup with a set of unnamed
  // generators


  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors                                        //
  //                                                   //
  ///////////////////////////////////////////////////////

  Type type( );

  // Inherited from FGGroup:
  // Chars FGGroup::nameOfGenerator( int i ) const;
  // VectorOf<Chars> FGGroup::namesOfGenerators( ) const;

  const FGGroup& parentGroup( ) const;

  const VectorOf<Word>& generators( ) const;

  void setParentGroup( const Group& g ) ;

  void setGenerators( const VectorOf<Word>& s ) ;

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Subgroup manipulators                            //
  //                                                   //
  ///////////////////////////////////////////////////////

  // NB manipulators which modify the semantics of the subgroup must
  // modify all necessary stored data to maintain consistency

  Subgroup& addGenerator( const Word& w );

  Subgroup& deleteGenerator( const Word& w );

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Group structure methods                          //
  //                                                   //
  ///////////////////////////////////////////////////////
 
  // Inherited from FGGroup:
  // int order( ) const;             // pseudo-virtual
  // Trichotomy isTrivial( ) const;  // pseudo-virtual
  // Trichotomy isFinite( ) const;   // pseudo-virtual
  // Trichotomy isInfinite( ) const; // pseudo-virtual
  // Trichotomy isAbelian( ) const;  // pseudo-virtual
 
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from FGGroup:
  // Elt makeIdentity( ) const;                               // pseudo-virtual
  // Bool isSyntacticIdentity(const Elt& e) const;            // pseudo-virtual
  // Trichotomy areEqual(const Elt& e1, const Elt& e2) const; // pseudo-virtual
  // Elt firstElt( ) const;                                   // pseudo-virtual
  // Elt nextElt(const Elt& e) const;                         // pseudo-virtual
  // Elt multiply(const Elt& e1, const Elt& e2) const;        // pseudo-virtual
  // Elt inverseOf(const Elt& e) const;                       // pseudo-virtual
  // Elt raiseToPower(const Elt& e, int n) const;             // pseudo-virtual
  // Elt conjugateBy(const Elt& e1, const Elt& e2) const;     // pseudo-virtual
  // Elt commutator(const Elt& e1, const Elt& e2) const;      // pseudo-virtual
  // Elt eval( const Word& w ) const;                         // pseudo-virtual
  // Trichotomy wordProblem( const Word& w ) const;           // pseudo-virtual
  // Trichotomy conjugacyProblem( const Word& u, const Word& v ) const;
                                                              // pseudo-virtual

  Word findRelator( ) ;
  // Attempts to find and return a relator for this group.
  // The words returned by successive calls will be distinct, but
  // they may be redundant as relators.
  // @rn We could use some means of querying whether this has any
  //     chance at all, e.g., is there at least a partial soln of the
  //     word problem for the parent group?

  Bool redundantRelator(const Word& u);
  
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with sets of group elements   //
  //                                                   //
  ///////////////////////////////////////////////////////
 
  // Inherited from FGGroup:
  // SetOf<Elt> setMultiply(const SetOf<Elt>& S1, const SetOf<Elt>& S2) const;
  // SetOf<Elt> setMultiply(const Elt& e, const SetOf<Elt>& S) const;
  // SetOf<Elt> setMultiply(const SetOf<Elt>& S, const Elt& e) const;
  // SetOf<Elt> conjugateBy(const SetOf<Elt>& S1, const SetOf<Elt>& S2) const;
  // SetOf<Elt> conjugateBy(const Elt& e, const SetOf<Elt>& S) const;
  // SetOf<Elt> conjugateBy(const SetOf<Elt>& S, const Elt& e) const;
  // void closeUnderInverses(SetOf<Elt>& S) const;
  // void closeUnderCyclicPermutations(SetOf<Word>& S) const;
 
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Representation access methods                    //
  //                                                   //
  ///////////////////////////////////////////////////////

///Inherited From FGGroup

 /*
private:

  // Shadow representation accessors to get representations of the
  // right type in the members of this class:

  const SubgroupRep* look( ) const {
	 return (const SubgroupRep*)GenericObject::look();
  }
  SubgroupRep* enhance( ) const {
	 return (SubgroupRep*)GenericObject::enhance();
  }
  SubgroupRep* change( ) {
	 return (SubgroupRep*)GenericObject::change();
  }

  // Special wrapping constructor to wrap new representations (returned
  // by eg. delegated methods) and for GenericObject initialisation by derived
  // classes:

  Subgroup( SubgroupRep* newrep ) : FGGroup(newrep) { }
*/
};

/* use ReadMeFrom()
%inline %{
	Subgroup * SubgroupFromString(char* p) {
		strstream s;
		Subgroup* F=new Subgroup();
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

*/

