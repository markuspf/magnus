///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "Range.h"
%}

struct Range {

  int low, high;
  bool Default;
  
  Range();

  %name(RangeFromInt) Range( int lo, int hi ) ;

///  bool operator == ( Range r ) const 
   %addmethods{
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
	int __cmp__(const Range& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}

  }
};
%inline %{
	Range * RangeFromString(char* p) {
		strstream s;
		Range* F=new Range();
		s << p << '\0' << flush;
		s >> *F;
		return F;
	}
%}
/*
// IPC tools:

void operator < ( ostream& ostr, const Range& g );
void operator > ( istream& istr, Range& g );


// I/O routines for class Range.

ostream& operator << ( ostream& ostr, const Range& g );
istream& operator >> ( istream& istr, Range& g );

*/
