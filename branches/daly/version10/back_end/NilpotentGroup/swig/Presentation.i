%module Magnus

%{
#include "Presentation.h"
%}

/*abstract class
class NilpotentPresentation {

public:

  //---------------------------------------------------------------------
  //  Constructors
  //---------------------------------------------------------------------

  NilpotentPresentation();
  // default constructor creates empty presentation.
  // The only purpose of this constructor is to allocate space for the
  // object. The object must be initialized by assignment.

//  %name(NilpotentPresentationFromNGCollector) NilpotentPresentation(const NGCollector& coll,const VectorOf<Word>& rels);
 

  // copy constructor, destructor and assignment operator provided by compiler


  //---------------------------------------------------------------------
  //  User interface
  //---------------------------------------------------------------------

  bool isBuilt() const ;
  // returns true iff the presentation is built

  void build();
  // to build the presentation completely

//  void print( ostream& s ) const;
  // output presentation to the stream s

  class FPGroup makeGroup() const;
  // produces a group using the presentation


  //---------------------------------------------------------------------
  //   IPC tools
  //---------------------------------------------------------------------

//  virtual ostream& write(ostream& s) const;
//  void print( ostream& s ) const;
  virtual istream& read(istream& s);
   %addmethods{
 	char* printOn() const {
		strstream *s=new strstream();
		self->print(*s);
		(*s) << '\0' << flush;
		return s->str();
	}
 	char* write() const {
		strstream *s=new strstream();
		self->write(*s);
		(*s) << '\0' << flush;
		return s->str();
	}

   }


  //---------------------------------------------------------------------
  //  Helper methods to form and use the presentation
  //---------------------------------------------------------------------

protected:

  SetOf<Word> convertRelators() const;
  // converts the presentation relators to Words

  Chars printRelator(int i) const;
  // outputs the ith relator in terms of theGenerators

  void buildKernelRelators();
  // builds relators di = 1.

  void improvePresentation();
  // extract identity generators from presentation

  //---------------------------------------------------------------------
  //  Virtual helper methods specific for each kind of presentation
  //---------------------------------------------------------------------

  virtual PolyWord decompose( const PolyWord& pw ) const = 0;
  // decomposes the word given in terms of the group generators
  // the result is the word in terms of theGenerators.

  virtual void printGenerators( ostream& s ) const = 0;
  // outputs the generators as a word s in terms of the group's generators

  //---------------------------------------------------------------------
  //  Data
  //---------------------------------------------------------------------

protected:

  VectorOf<PolyWord> theGenerators;
  //theGenerators are words in terms of the group generators
  //They are set within constructor

  VectorOf<Chars> theNames;
  // the names for the words from theGenerators
  //They are set within constructor

  VectorOf<NilpotentRelator> theRelators;
  // The nilpotent relators form presentation.
  //They are set within build() method.

  bool built;
  // Indicates whether the presentation is built.

  VectorOf<Word> theGroupRelators;
  // The relators of the nilpotent group
  
  NGCollector theCollector;
  //the group collector

};
*/

//=======================================================================
//====================== PresentationForNG ==============================
//=======================================================================

// The presentation for nilpotent group
// theGenerators are just 1,2, ....
// theNames are c1,c2, ....
// theRelators are all commutation relators plus
//     di = 1 where di are words of Malcev basis for the relation subgroup
//     (in FPNilpotentGroup case)




class PresentationForNG  {

public:

  //---------------------------------------------------------------------
  //  Constructors
  //---------------------------------------------------------------------

  PresentationForNG(const NGCollector& coll, const VectorOf<Word>& rels);
//Inherited from NilpotentPresentation
  //---------------------------------------------------------------------
  //  User interface
  //---------------------------------------------------------------------

  bool isBuilt() const ;
  // returns true iff the presentation is built

  void build();
  // to build the presentation completely

//  void print( ostream& s ) const;
  // output presentation to the stream s

  class FPGroup makeGroup() const;
  // produces a group using the presentation


  //---------------------------------------------------------------------
  //   IPC tools
  //---------------------------------------------------------------------

//  virtual ostream& write(ostream& s) const;
//  void print( ostream& s ) const;
  virtual istream& read(istream& s);
   %addmethods{
 	char* printOn() const {
		strstream *s=new strstream();
		self->print(*s);
		(*s) << '\0' << flush;
		return s->str();
	}
 	char* write() const {
		strstream *s=new strstream();
		self->write(*s);
		(*s) << '\0' << flush;
		return s->str();
	}

   }

};



//=======================================================================
//====================== PresentationForSNG =============================
//=======================================================================

// The presentation for subgroup of nilpotent group
// theGenerators are the words of Malcev basis for the full preimage
// theRelators are all commutation relators plus
//     di = 1 where di are words of Malcev basis for the relation subgroup
//     (in SGOfFPNilpotentGroup case)


class PresentationForSNG  {

public:

  //---------------------------------------------------------------------
  //  Constructors
  //---------------------------------------------------------------------

  PresentationForSNG(const MalcevSet& preimage, const VectorOf<Word>& rels,
		     const NGCollector& coll);
//Inherited from NilpotentPresentation
  //---------------------------------------------------------------------
  //  User interface
  //---------------------------------------------------------------------

  bool isBuilt() const ;
  // returns true iff the presentation is built

  void build();
  // to build the presentation completely

//  void print( ostream& s ) const;
  // output presentation to the stream s

  class FPGroup makeGroup() const;
  // produces a group using the presentation


  //---------------------------------------------------------------------
  //   IPC tools
  //---------------------------------------------------------------------

//  virtual ostream& write(ostream& s) const;
//  void print( ostream& s ) const;
  virtual istream& read(istream& s);
   %addmethods{
 	char* printOn() const {
		strstream *s=new strstream();
		self->print(*s);
		(*s) << '\0' << flush;
		return s->str();
	}
 	char* write() const {
		strstream *s=new strstream();
		self->write(*s);
		(*s) << '\0' << flush;
		return s->str();
	}

   }

};


