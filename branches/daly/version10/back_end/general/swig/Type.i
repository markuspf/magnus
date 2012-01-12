%module Magnus

%{
#include "Type.h"
%}
%inline %{
    Type *new_UNIQUETYPE(){
        return new Type(Type::unique());
    }
    Type *new_NOTYPE(){
        return new Type(Type::notype());
    }
%}

class Type {
public :

// constructors:

	class notype { }; // pure `marker' helper class
//	class unique { }; // pure `marker' helper class
    ///use NOTYPE() and UNIQUETYPE instead of constructor.
    Type( const notype& );
	// default constructor yields Type object guaranteed not to
    // represent type of any class

//    %name(UNIQUETYPE) Type( const unique&);
	// special constructor for initialisation of the client class static
	// Type member

	// copy constructor provided by compiler

	// destructor provided by compiler
// operators:

	// operator = provided by compiler


// I/O:

///	void writeTo( ostream& o );
////use __str__

	void readMeFrom( istream& i );
 
    %addmethods {
     ///    int operator== ( const Type& t) { return typeNumber == t.typeNumber; }
     ///    int operator!= ( const Type& t) { return !(*this == t); }
	int __cmp__(const Type& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
     ///inline ostream& operator << ( ostream& o, const Type& t )
       	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
     ///inline istream& operator >> ( istream& i, Type& t )
     ///use readMeFrom
    }

};



 

