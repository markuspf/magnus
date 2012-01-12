%module Magnus

%{
#include "BasicCommutators.h"
%}




//====================================================================
//================ class BasicCommutators ============================
//====================================================================


class BasicCommutators {

public:


  //------------------------------------------------------------------
  //  Constructors and initialization
  //------------------------------------------------------------------

  BasicCommutators(int numgen, int nilclass, bool initialize = true);
  // To construct the set of basic commutators which constitute a Mal'cev
  // basis for free nilpotent group of `numgen' generators and nilpotency
  // class `nilclass'.
  // Flag initialize shows whether the initialization should be
  // performed immediately or delayed

  //copy constructor, destructor and assignment operator provided by compiler


  void initialize() const;    // Logical const!
  // initializes uninitialized object


  //------------------------------------------------------------------
  //  Methods dealing with the whole set of basic commutators
  //  (accessors)
  //------------------------------------------------------------------

  int theHirschNumber() const;
  // the Hirsch number (the number of basic commutators)

  int numberOfGenerators() const;
  // the number of generators

  int nilpotencyClass() const ;
  // the nilpotency class

  int numberOfWeight(int i) const;
  // The number of basic commutators of given weight

  int theFirstOfWeight(int i) const ;
  // The number of first basic commutator of given weight
  

  bool isInitialized() const ;
  // true iff the structure is initialized

  class NGWordForms wordForm() const;   // inline
  // produces an object of helper class


  //------------------------------------------------------------------
  //  Methods dealing with basic commutators
  //------------------------------------------------------------------

  // Here basic commutators are given by indices of corresponding
  // entries in the table


  //--------------- access to BCEntry properties ---------------------

  int weightOf(int i) const ;

  int leftIndexOf(int i) const ;

  int rightIndexOf(int i) const ;

  virtual Chars commutatorName(int i) const ;
  // default name for basic commutator, can be overridden
    
  VectorOf<Chars> commutatorNames() const;
  // names of basic commutators

  //-------------- properties of commutator [g,h] --------------------

  bool commutatorIsBasic(int g, int h) const;
  // returns true iff the commutator [g,h] is proper basic.

  int findBC(int g, int h) const;
  // returns the index of the commutator [g,h].
  // If the commutator is not basic, generates an error.
  // If [g,h] = 1, returns 0.

  bool generatorsCommute(int g, int h) const ;
  // return true iff [g,h] = 1


  //------------- methods for collecting process ----------------------


  PolyWord commuteLetters(const Letter& left, const Letter& right) const;
  // If left.gen < right.gen, returns the word ( left + right ).
  // If left.gen == right.gen, collects them.
  // If left.gen > right.gen, commutes the letters.
  // "Commutes" means: result.firstLetter().gen < left.gen.

  PolyWord findCommutationRelation( Letter Cj, Letter Ci ) const;
  // finds a commutation relation for Ci^-1 Cj Ci
  // Note: only signs of powers are taken into account, not powers themselves

  PolyWord decomposeLetter(Letter C) const;
  // decomposes the given letter [a,b]^p into a PolyWord:
  // a^-1 b^-1 a b  a^-1 b^-1 a b  ...  a^-1 b^-1 a b (p times)
///  virtual void print( ostream& BClist, const VectorOf<Chars>& genNames ) const;
  %addmethods{
    	char* printOn(const VectorOf<Chars>& genNames) const{
		strstream *s=new strstream();
		self->print(*s,genNames);
		(*s) << '\0' << flush;
		return s->str();
	}
  }
/*
  //------------------------------------------------------------------
  //  I/O and debugging stuff
  //------------------------------------------------------------------


  friend ostream& operator < ( ostream& ostr, const BasicCommutators& BC );
  friend istream& operator > ( istream& istr, const BasicCommutators& BC );
  // IPC tools

  virtual void print( ostream& BClist, const VectorOf<Chars>& genNames ) const;
  // outputs basic commutators to a file

  //------------------------------------------------------------------
  //   Friends:
  //------------------------------------------------------------------

  friend class NGWordForms;

  //------------------------------------------------------------------
  //   Data members:
  //------------------------------------------------------------------


private:

  int nilClass;
  // the nilpotency class

  int numGens;
  // the number of generators

  int theNumberOfBC;
  // the number of basic commutators

  bool initialized;
  // true iff the commutators are built

  VectorOf<int> firstOfWeight;
  // for fast access to basic commutators of certain weight.
  // firstOfWeight[i] is the index of the first basic commutator of weight i.
  // 1-based indices ( well, we don't have any BC's of weight less than 1. )

  VectorOf<BCEntry> entries;
  // ordered array of all basic commutators.
  // 1-based indices.
*/
};

class NGWordForms {

public:

  NGWordForms(const BasicCommutators& bc);
  // The only constructor (except copy constructor)


  //@ep I deliberately use no conversion operators to avoid
  //    undesirable implicit conversions


  //-----------------------------------------------------------------
  //  Conversions to the Word
  //-----------------------------------------------------------------

  // A result of the conversion is a Word in terms of the group
  // generators

  Word toWord(int commIndex) const;
  // The argument is a commutator index

  %name(toWordFromPolyWord) Word toWord(const PolyWord& w) const;
  // The argument is a PolyWord in terms of basic commutators

  %name(toWordFromWord) Word toGroupWord(const Word& basicWord) const;
  // The argument is a Word given in terms of basic commutators


  //-----------------------------------------------------------------
  //   Conversions to the text string
  //-----------------------------------------------------------------


  //--- to string in terms of basic commutators (c1, c2, c3....) ----


  Chars commutatorStructure(int commIndex) const;
  // If weight(commIndex) == 1, outputs generatorName(commindex).
  // Else outputs "[cj,ci]".

  Chars asCommutatorWord(const PolyWord& w) const;
  // Outputs the given word in terms of basic commutator names:
  //           c1 c2^-3 c4^2

  %name(asCommutatorWordFromLetter) Chars asCommutatorWord(Letter s) const;
  // Outputs the given letter in terms of basic commutator names:
  //            c2^-3


  //---- to string in terms of group generators ----------------------
  //  Names of the generators are provided


  Chars asBracketedWord(int commIndex, const VectorOf<Chars>& genNames,
			bool embeddingBrackets = true) const;
  // Produces a string representing the given basic commutator commIndex
  // as a word of genNames in commutator form, e.g.:
  //       [x,[x,y],z]
  // The parameter embeddingBrackets defines whether the outermost
  // brackets should present.

  %name(asBracketedWordFromPolyWord) Chars asBracketedWord(const PolyWord& w, const VectorOf<Chars>& genNames) const;
  // Outputs PolyWord in the bracketed form (in terms of group generators)
/*

private:

  const BasicCommutators& BC;
*/
};















