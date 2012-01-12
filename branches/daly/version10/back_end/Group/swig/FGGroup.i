%module Magnus

%{
#include "FGGroup.h"
#include "DerivedObjectOf.h"
#include "Group.h"
#include "FGGroupRep.h"
%}

class Subgroup; // Doesn't exist yet.

class FGGroup:public Group{
  
public:
  
  FGGroup( const Group& g );
  Type type( );
  // dominates Group::type();

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
  // int order( ) const; // pseudo-virtual
  // Trichotomy isTrivial( ) const; // pseudo-virtual
  // Trichotomy isFinite( ) const; // pseudo-virtual
  // Trichotomy isInfinite( ) const; // pseudo-virtual
  // Trichotomy isAbelian( ) const; // pseudo-virtual

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from Group:
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

  // Inherited from Group:
  // SetOf<Elt> setMultiply(const SetOf<Elt>& S1, const SetOf<Elt>& S2) const;
  // SetOf<Elt> setMultiply(const Elt& e, const SetOf<Elt>& S) const;
  // SetOf<Elt> setMultiply(const SetOf<Elt>& S, const Elt& e) const;
  // SetOf<Elt> conjugateBy(const SetOf<Elt>& S1, const SetOf<Elt>& S2) const;
  // SetOf<Elt> conjugateBy(const Elt& e, const SetOf<Elt>& S) const;
  // SetOf<Elt> conjugateBy(const SetOf<Elt>& S, const Elt& e) const;
  // void closeUnderInverses(SetOf<Elt>& S) const;

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
///disabled because of ostream
//  void printGenerator( ostream& ostr, const Generator& g ) const;
  // Outputs the print name of g on ostr, followed by "^-1" if ord(g) < 0.

//  void printGenerators( ostream& ostr ) const;
  // Calls printGenerator for each generator of this group, separating
  // them with commas.

//  void printWord( ostream& ostr, const Word& w ) const;
  // Outputs w to ostr, in a form that can input by readWord.

//  void printSetOfWords( ostream& ostr, const SetOf<Word>& S ) const;
  // Outputs S to ostr, in a form that can input by readWord.

//  void printVectorOfWords( ostream& ostr, const VectorOf<Word>& V,
//	    char* leftBracket = "{", char* rightBracket = "}" ) const;
  // Outputs V to ostr, in a form that can input by readWord.



  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with subgroups                //
  //                                                   //
  ///////////////////////////////////////////////////////

  //deleted because of no defination
 // Subgroup randomSubgroup( ) const; // @stc this has to be set up with
                              // handles to manipulate distributions

  %addmethods{
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
};

/*
%inline %{
	FGGroup * FGGroupFromString(char* p) {
		strstream s;
		FGGroup* F=new FGGroup();
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