%module Magnus

%{
#include "AbelianWord.h"
%}

class AbelianWord 
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Constructors:                                                      //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // Copy constructor provided by compiler (does logical deep copy).

  AbelianWord( );
  ///numOfGens=size(Word& w)
  %name(AbelianWordFromWord) AbelianWord( int numOfGens, const Word& w );

  %name(AbelianWordFromVector) AbelianWord( const VectorOf<Integer>& v );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Operators:                                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Accessors:                                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int numberOfGenerators( ) const ;
  // Returns the number of different generators in the word presentation.

  VectorOf<Integer> getPowers( ) const ;
  // Transforms an abelian word to a vector of generators powers

  Word getWord( ) const ;
  // Transforms an abelian word to an usual word. 

  Integer fullLength( ) const;
  // I.e. the sum of all powers.

  AbelianWord inverse( ) const ;
  // Computes the inverse word.

  bool isTrivial( ) const;
///xxw this is only a test function testing Integer mapping
  Integer test(Integer orderofElt) const;
//friend ostream& operator << ( ostream& ostr, const AbelianWord& w );
  

//friend ostream& operator < ( ostream& ostr, const AbelianWord& w );
  
//friend istream& operator > ( istream& istr, AbelianWord& w );
   %addmethods {
       
            //  bool operator == ( const AbelianWord& w ) const ;
            //  int operator != ( const AbelianWord& w ) const ;
	int __cmp__(const AbelianWord& w) {
              // only makes sense for == and !=, not for > or <
           if ((*self)==w)
		return 0;
           return 1;
	}

           //  Integer operator [] ( int i ) const;
           //  Integer& operator [] ( int i ) ;
	Integer __getitem__(int i) {
		return (*self)[i];
	}


           //  AbelianWord operator * ( const AbelianWord& w ) const;
	AbelianWord __mul__(const AbelianWord& w) {
		return (*self)*w;
	}
           //friend ostream& operator << ( ostream& ostr, const AbelianWord& w );
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
   }
};
