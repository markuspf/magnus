%module Magnus

%{
#include "LCSQuotients.h"
%}


struct BasisWord {

  //-------- Fictive operators for VectorOf templates ----------------
/*  no defination
  friend bool operator == (const BasisWord& w1, const BasisWord& w2);
  friend ostream& operator << (ostream& s, const BasisWord& w);*/
/*

  //-------- IPC tools -----------------------------------------------

  friend ostream& operator < ( ostream& s, const BasisWord& w );
  // IPC output

  friend istream& operator > ( istream& s, BasisWord& w );
  // IPC input

  //-------- Data ----------------------------------------------------
*/

  PolyWord theWord;
  //the word in terms of basic commutators

  int theWeight;
  //its weight in the group

  int theOrder;
  //its order (0 means infinite)

  int theLocalNumber;
  // its ordinal number in the LCS quotient, 1-based
  // (basic words of finite and infinite orders are numerated separately) 

  int theGlobalNumber;
  // its ordinal number in the group basis, 1-based

  Chars theName;
  // the name of the word

};

class LCSQuotient {

public:

  //----------------------------------------------------------------------
  //  Constructors / initializers
  //----------------------------------------------------------------------
  
  LCSQuotient();
  //default constructor 


//  friend bool operator == (const LCSQuotient& q1, const LCSQuotient& q2);
  // comparison operator (fictive to fool the VectorOf templates

//  friend ostream& operator << (ostream& s, const LCSQuotient& q);
  // to fool the VectorOf templates

 // friend ostream& operator < ( ostream& s, const LCSQuotient& q );
  // IPC output

//  friend istream& operator > ( istream& s, LCSQuotient& q );
  // IPC input

  //--------------------------------------------------------------------
  //   Data
  //--------------------------------------------------------------------

  AbelianGroup abelianization;
  VectorOf<BasisWord> generators;
  int infNumber;
  int numerationShift;
  MalcevSet basis;
//  friend bool operator == (const LCSQuotient& q1, const LCSQuotient& q2);
//  friend ostream& operator << (ostream& s, const LCSQuotient& q);
 %addmethods {

/*no defination
	int __cmp__(const LCSQuotient& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}

	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
*/

}

};


