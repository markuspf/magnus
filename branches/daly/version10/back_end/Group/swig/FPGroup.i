/*
FPGroup.i
$Id$

This one is pretty straightforward. The list of methods that get wrapped
may have to be rethought at some point. Note that inherited functions have
to be wrapped here if they are supposed to be available to FPGroup.

xiaowei xu, 05/30/2001
xuxw76@yahoo.com
*/

%module Magnus

%{
#include "FPGroup.h"
%}

// class Aut; // Doesn't exist yet


class FPGroup :public FGGroup{

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors                                     //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Copy constructor provided by compiler

  FPGroup( int ngens = 0 );
  // To construct a group of given number of unnamed generators and
  // no relators. Default ngens = 0 gives trivial group with 0
  // generators

  %name(FPGroupFromSet) FPGroup( int ngens, const SetOf<Word>& rels );
  // To construct a group of given number of unnamed generators and
  // given relators.
  
  %name(FPGroupFromVector) FPGroup( const VectorOf<Chars>& gennames );
   // To construct a group with named generators, no relators.

  %name(FPGroupFromVectorSet) FPGroup( const VectorOf<Chars>& gennames, const SetOf<Word>& rels );
  // To construct a group with named generators, given relators.
  //  %name(FPGroupFromGroup) FPGroup( const Group& G );
  %name(FPGroupFromGroup) FPGroup( const FPGroup& G );
  // cast construtor: to cast an arbitrary group as an FP group.
  // NB. This rewraps the unchanged representation, hence is in general
  // only useful for casting a group known to be an actual FP group.

  // @stc FPGroup( const FreeGroup& G ) :
  //	FGGroup(G) { } // @stc bugged constructor
  // To make a free group into a finitely presented one.
  // @stc if you used this constructor, check your code -- all it did
  // was rewrap an Free gp rep as an FP gp.
  
  // Destructor provided by compiler.

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors / Modifiers                            //
  //                                                   //
  ///////////////////////////////////////////////////////
  
  // Inherited from Group:
  //@rn how to do this systematically?
  // @stc I don't think there is any proper way to do it systematically,
  // any class anywhere may have access adjusters: thus the only way to
  // `see' the complete interface in every class is to write it down
  // exhaustively; however, each inherited method should be considered
  // inherited from its immediate parent, in order not to make the
  // validity of the statements here dependent on changes higher up.
  // it's probably better to put each method in its proper contextual place

  Type type( ) ;
  // dominates FGGroup::type();

  // Inherited from FGGroup:
  //  Type actualType() const; // pseudo-virtual
  //  int numberOfGenerators( ) const;
  //  Chars nameOfGenerator(int i) const;
  //  VectorOf<Chars> namesOfGenerators( ) const;

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

  Trichotomy isFree( ) const ;
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

///xxw this is only a test function testing Trichotomy mapping
  Trichotomy test(int order) const;
  SetOf<Word> testSet(SetOf<Word>& source) const;
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
  }
};


%inline %{
	FPGroup * FPGroupFromString(char* p) {
		strstream s;
		FPGroup* F=new FPGroup();
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