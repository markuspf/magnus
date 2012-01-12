%module Magnus

%{
#include "RipsConstruction.h"
%}

class RipsConstruction {

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  RipsConstruction( const FPGroup& G );
  // Default & copy contructors, operator = provided by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

//  FPGroup getRipsConstruction( ostream* out = NULL ) const;

/*

private:
 
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  FPGroup theGroup;
*/
  %addmethods{
      	char* getRipsConstruction() const {
		strstream *s=new strstream();
		self->getRipsConstruction(s);
		(*s) << '\0' << flush;
		return s->str();
	}
   }
};

