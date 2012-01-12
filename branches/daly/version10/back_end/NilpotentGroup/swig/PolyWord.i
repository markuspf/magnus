%module Magnus

%{
#include "PolyWord.h"
%}
class PolyWord {

  //--------------------------------------------------------
  //	Constructors
  //--------------------------------------------------------

public:
  
  PolyWord();

  %name(PolyWordFromLetter) PolyWord( const Letter& let) ;

  %name(PolyWordFromWord) PolyWord( const Word& w) ;

  %name(PolyWordFromAbelianWord)  PolyWord( const AbelianWord& aw) ;

  %name(PolyWordFromPolyWordRep)  PolyWord(PolyWordRep* rep) ;


  // copy constructor and destructor provided by compiler

  //--------------------------------------------------------
  //	Accessors to word properties
  //--------------------------------------------------------

  int length() const ;
  // length of the word (sum of absolute values of powers).

  int numberOfLetters() const ;
  // number of Letters (length of the list)

  bool isEmpty() const ;

  bool isCollected() const ;
  // returns true iff the word is collected

  //--------------------------------------------------------
  //	Accessors to word elements
  //--------------------------------------------------------

  const Letter& firstLetter() const ;
  // returns reference to the first letter

  const Letter& lastLetter() const ;
  // returns reference to the last letter

  // To access the other word elements, use PolyWordIterator
//  friend class PolyWordIterator;
//  friend class ConstPolyWordIterator;

  //--------------------------------------------------------
  //	I/O
  //--------------------------------------------------------

//  void printOn (ostream& s) const;
  // output

//  friend ostream& operator < (ostream& s, const PolyWord& w) ;
  // IPC output

//  friend istream& operator > (istream& s, PolyWord& w) ;
  // IPC input
 
  void debugInfo() const;
  // Prints debug info to cout

 
  //--------------------------------------------------------
  //	General methods
  //--------------------------------------------------------

//  operator Word() const;
  // cast to common Word (not reduced)

  Chars toChars( const VectorOf<Chars>& names ) const;
  // outputs word to string using given Chars as names of generators

  void freelyReduce() ;
  // freely reduces the word

  PolyWord inverse() const ;
  // returns the inverse word

  void append(const PolyWord& w) ;
  // appends w to the end of this word

  %name(appendFromLetter) void append(const Letter& w) ;
  // appends w to the end of this word

  void clear() ;
  // clears the list

//  void duplicate(PolyWordNode*& ptrToFirst, PolyWordNode*& ptrToLast) const;
  // duplicates the list and writes addresses of the new copy's
  // first and last nodes to ptrToFirst and ptrToLast.

  void removeFirstLetter() ;
  // removes first letter (node) from the list

  PolyWord raiseToPower( int power ) const ;
  // Raises the word to given power (can be negative or 0)
//inline ostream& operator<<(ostream& s, const PolyWord& w) 
// stream output

//inline PolyWord operator * (const PolyWord& p1, const PolyWord& p2) 
   %addmethods {

	Word __mul__(const PolyWord& w) {
		return (*self)*w;
	}

	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
///  void printOn (ostream& s) const;
	char* printOn() const {
		strstream *s=new strstream();
		self->printOn(*s);
		(*s) << '\0' << flush;
		return s->str();
	}

   }
};

//----------------- Inline global functions -----------------
/*
inline ostream& operator<<(ostream& s, const PolyWord& w) 
// stream output

inline PolyWord operator * (const PolyWord& p1, const PolyWord& p2) 
// catenation of words

*/

%name(PolyWordCommutator) PolyWord commutator(const PolyWord& w1, const PolyWord& w2);
// returns commutator [w1, w2]

%name(PolyWordCommutatorOfInverses) PolyWord commutatorOfInverses(const PolyWord& w1, const PolyWord& w2);
// returns commutator [w1^-1, w2^-1]

%name(PolyWordEvaluateWordFromWord) PolyWord evaluateWord( const Word& w, const VectorOf<PolyWord>& pw );
// Evaluates the word w by replacing its generators ai -> pw[i].

%name(PolyWordEvaluateWord) PolyWord evaluateWord(const PolyWord& pw, const VectorOf<PolyWord>& v);
//Evaluates the word replacing generator g by word v[g-1].
//@ep cannot make it a class member due to gcc template bugs





