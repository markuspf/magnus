/*

Generator.i
$Id$

Note: This file doesn't expose many methods of the class Generator. Chances
are that users of the scripting interface will never use this class. From
the point of view of users, the class Word supercedes Generator.

Peter Brinkmann, 05/18/2001
brinkman@math.uiuc.edu
*/

%module Magnus

%{
#include "Generator.h"
#include "Elt.h"
%}

class Generator {

public:
  
  // copy constructor compiler-supplied
  // destructor compiler-supplied
  // assignment operator compiler-supplied

  // Generator( ) : genrep(0) { }
  // A default constructor for making, e.g., arrays of uninitialized
  // Generators. With SAFETY > 0, the Word constructor which takes arrays of
  // Generators checks for uninitialized ones.

  Generator( int i );
  // @rn Is it worthwhile to refuse negative args, thus forcing the use
  // of inv?

  // Generator has no non-static members in its public interface
  // in order to have the use of a Genref wherever a Generator is expected
  // (the compiler would not understand g.member() if g were a Genref
  // and member() in Generator).
    
//  inline friend int ord( Generator g );
  // this is the only friend; all other functions in the public interface
  // of Generator are defined in terms of ord()

  int hash() const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

%addmethods{
	int __cmp__(const Generator& w) {
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
}

};


// functions for manipulating Generators

// the notion of inverse:

Generator inv( Generator g );

// comparators:

// inline int operator == ( Generator g, Generator h ) { return ord(g) == ord(h); }
// 
// inline int operator != ( Generator g, Generator h ) { return ord(g) != ord(h); }
// 
// inline int operator < ( Generator g, Generator h ) { return ord(g) < ord(h); }
// 
// inline int operator <= ( Generator g, Generator h ) { return ord(g) <= ord(h); }
// 
// inline int operator > ( Generator g, Generator h ) { return ord(g) > ord(h); }
// 
// inline int operator >= ( Generator g, Generator h ) { return ord(g) >= ord(h); }

// I/O

// Generator properties

int power(Generator g);

int abs(Generator g);
// The absolute value of generator representation

int shortLexIndex(Generator g);
int ord( Generator g );
// Return the generator's index in ShortLex ordering:
//   1  -> 0
//  -1  -> 1
//   2  -> 2
//  -2  -> 3
//  ........
