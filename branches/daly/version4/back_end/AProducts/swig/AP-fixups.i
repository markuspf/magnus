///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "AP-fixups.h"
%}

struct RelatorConjugate 
{

  RelatorConjugate( ) ;
  %name(RelatorConjugateFromWord) RelatorConjugate( const Word& r, const Word& c );

  // Copy constructor, operator= and destructor are supplied by compiler.
  //bool operator==( const RelatorConjugate& t) const ;

  // Data members:
  Word relator, conjugator;
  %addmethods{
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
  }
};

// Standard I/O routines.
//inline ostream& operator<<( ostream& ostr, const RelatorConjugate& rc )

//inline istream& operator>>( istream& istr, RelatorConjugate& rc )

/*
// IPC support.
inline ostream& operator<( ostream& ostr, const RelatorConjugate& rc )
{
  ostr < rc.relator < rc.conjugator;
  return ostr;
}
inline istream& operator>( istream& istr, RelatorConjugate& rc )
{
  istr > rc.relator > rc.conjugator;
  return istr;
}

*/

typedef VectorOf<RelatorConjugate> ProductOfRelatorConjugates;

ProductOfRelatorConjugates conjugateBy( 
  const ProductOfRelatorConjugates& product, const Word& conjugator );

%inline %{
	RelatorConjugate * RelatorConjugateFromString(char* p) {
		strstream s;
		RelatorConjugate* F=new RelatorConjugate();
		s << p << '\0' << flush;
		return F;
	}
%}