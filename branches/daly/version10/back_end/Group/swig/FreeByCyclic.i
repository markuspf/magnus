%module Magnus

%{
#include "FreeByCyclic.h"
%}

class FreeByCyclic
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  FreeByCyclic(const FreeGroup F, const Map aut);

  // Destructor supplied by compiler

//  FreeByCyclic& operator=( const FreeByCyclic& G );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  VectorOf<Chars> namesOfGenerators( ) const;

  SetOf<Word> getRelators( ) const;

  Word normalForm(const Word& w) const;

//  friend ostream& operator<<( ostream& ostr, const FreeByCyclic& G );
//  friend istream& operator>>( istream& istr,       FreeByCyclic& G );
/*
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const FreeGroup theGroup;

  const Map theAut;
  const Map  theAutInv;

  int newGenerator;
*/
//  friend ostream& operator<<( ostream& ostr, const FreeByCyclic& G );
   %addmethods {
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
//  friend istream& operator>>( istream& istr,       FreeByCyclic& G );
        void ReadMeFrom(istream& i){
            i>>*self;
         }
   }
};
