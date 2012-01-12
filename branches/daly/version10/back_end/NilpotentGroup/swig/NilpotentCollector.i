
%module Magnus

%{
#include "NilpotentCollector.h"
%}

//  not mapped because it's an abstract class

class NilpotentCollector
{

public:

  enum Type { STANDARD = 0, TO_THE_LEFT = 0, FROM_THE_LEFT = 1,
	      FROM_THE_RIGHT = 2, POLYNOMIAL = 3
  };  // to be extended

  //------------------------------------------------------------------------
  //   Constructors and initializers
  //------------------------------------------------------------------------

  NilpotentCollector(int numgen, int nilclass, bool initialize = true);
  // creates collector for free nilpotent group 
  // Flag initialize shows whether the initialization should be
  // performed immediately or delayed

  %name(NilpotentCollectorFromBasicCommutators) NilpotentCollector(const BasicCommutators& bc);
  //To create collector using given BC
  
  //copy constructor, destructor and assignment operator provided by compiler

  virtual void initialize() const;    // Logical const!
  // initializes uninitialized object

  //------------------------------------------------------------------------
  //   Factories
  //------------------------------------------------------------------------

  NilpotentCollector * make(int numgen, int nilclass,
				   NilpotentCollector::Type colType,
				   bool initialize = true);
  // makes a collector of given type


  virtual NilpotentCollector * clone() const=0;
  // method to clone the object


  //------------------------------------------------------------------------
  //   Accessors
  //------------------------------------------------------------------------

  const BasicCommutators& commutators() const;
  // access to the basic commutators

  virtual NilpotentCollector::Type type() const=0;
  // the collector type

  virtual bool isInitialized() const;
  // true iff the object is initialized


  //------------------------------------------------------------------------
  //   Common problems related with the collecting process
  //------------------------------------------------------------------------

  virtual void collectingProcess( PolyWord& pw ) const;
  // collects the given PolyWord. The result is stored in the same PolyWord.

  virtual PolyWord collect(const Word& w) const;
  // collects the word
  // Input word is given in generators of the nilpotent group.
  // The output is a PolyWord in generators of Malcev basis.

  %name(collectFromPolyWord) virtual PolyWord collect(const PolyWord& pw) const;
  // collects the PolyWord
  // The output is a PolyWord in generators of Malcev basis.

  virtual bool collectLetter(PolyWord& w, Generator c) const;
  // collects one letter
  // Returns true if the letter presents in the word after collection,
  // and false if the letter was eliminated or not found

  virtual PolyWord multiply(const PolyWord& pw1, const PolyWord& pw2) const;
  // returns collected product of two words

  virtual PolyWord raiseToPower(const PolyWord& pw, int power) const;
  // returns a collected power of the word

  virtual PolyWord inverse(const PolyWord& pw) const;
  // returns collected inverse of the word

  virtual int weightOf(const Word& w) const;
  // returns the weight of the word (the weight of the first letter
  // in the word's collected form)
  // (nilClass+1) means word is trivial
  
  %name(weightOfFromPolyWord) virtual int weightOf(const PolyWord& w) const;
  // returns the weight of the word (the weight of the first letter)
  
  //------------------------------------------------------------------------
  //   IPC tools
  //------------------------------------------------------------------------

//  virtual ostream& writeIPC(ostream& s) const;
//  virtual istream& readIPC(istream& s) const;  //Logical const!
  // IPC tools

  //------------------------------------------------------------------------
  //   Data
  //------------------------------------------------------------------------
/*
protected:

  BasicCommutators BC;
*/
  %addmethods{
      	char* writeIPC() const {
		strstream *s=new strstream();
		self->writeIPC(*s);
		(*s) << '\0' << flush;
		return s->str();
	}
      	void readIPC(istream& i) const {
             self->readIPC(i);
	}
   }
};


//===========================================================================
//============================ class NGCollector ============================
//===========================================================================


// This is a wrapper for NilpotentCollector family of classes.
// To be redesigned to use standard form for shared representations

class NGCollector {

public:


  //------------------------------------------------------------------------
  //   Constructors and initializers
  //------------------------------------------------------------------------

  NGCollector(int numgen, int nilclass, 
	      NilpotentCollector::Type collType = NilpotentCollector::STANDARD,
//	      int collType = 0,
	      bool initialize = true);
  // Creates a collector of given type
  

  //NGCollector(const BasicCommutators& bc, Type collType = STANDARD);
  // Creates a collector using existing set of commutators
  
  %name(NGCollectorFromNGCollector) NGCollector(const NGCollector& ngc);
  // The copy constructor


  ~NGCollector() ;
  //The destructor


//  NGCollector & operator = (const NGCollector& ngc) ;
  //The assignment operator


  void initialize() const;
  // initializes uninitialized object


  //------------------------------------------------------------------------
  //   Accessors
  //------------------------------------------------------------------------

  const BasicCommutators& commutators() const ;
  // access to the basic commutators

  NilpotentCollector::Type type() const ;
  // the collector type

  bool isInitialized() const ;
  // true iff the object is initialized

  //------------------------------------------------------------------------
  //   Common problems related with the collecting process
  //------------------------------------------------------------------------

  void collectingProcess( PolyWord& pw ) const ;
  // collects the given PolyWord. The result is stored in the same PolyWord.

  PolyWord collect(const Word& w) const ;
  // collects the word
  // Input word is given in generators of the nilpotent group.
  // The output is a PolyWord in generators of Malcev basis.

  %name(collectFromPolyWord) PolyWord collect(const PolyWord& pw) const ;
  // collects the PolyWord
  // The output is a PolyWord in generators of Malcev basis.

  bool collectLetter(PolyWord& w, Generator c) const ;
  // collects one letter
  // Returns true if the letter presents in the word after collection,
  // and false if the letter was eliminated or not found


  PolyWord multiply(const PolyWord& pw1, const PolyWord& pw2) const ;
  // returns a collected product of two words

  PolyWord raiseToPower(const PolyWord& pw, int power) const ;
  // returns a collected power of the word

  virtual PolyWord inverse(const PolyWord& pw) const ;
  // returns collected inverse of the word

  int weightOf(const Word& w) const ;
  // returns the weight of the word (the weight of the first letter
  // in the word's collected form)
  // (nilClass+1) means word is trivial
  
  %name(weightOfFromPolyWord) int weightOf(const PolyWord& w) const ;
  // returns the weight of the word (the weight of the first letter)
  
  //------------------------------------------------------------------------
  //   IPC tools
  //------------------------------------------------------------------------

//  ostream& writeIPC(ostream& s) const;

//  istream& readIPC(istream& s) const;  //Logical const!


  //------------------------------------------------------------------------
  //   Data
  //------------------------------------------------------------------------
  %addmethods{
    	char* writeIPC() const {
		strstream *s=new strstream();
		self->writeIPC(*s);
                (*s) << '\0' << flush;
                return s->str();
        }
  }
/*

private:

  NilpotentCollector * NC;
*/
};

/*

//====================================================================
//====================== free functions ==============================
//====================================================================

inline ostream& operator < ( ostream& ostr, const NGCollector& NGC ) {
  return NGC.writeIPC(ostr);
}

inline istream& operator > ( istream& istr, const NGCollector& NGC) {
  return NGC.readIPC(istr);
}
*/


