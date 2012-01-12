%module Magnus

%{
#include "TTP.h"
%}

class TTP {

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  TTP( const FPGroup& G );
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Chars getFileName( ) const;
  // returns the name of the presentation file

  bool run( ) const;
  // Enumerates Tietze transformations and writes corresponding
  // presentations to a file.  TIME CONSUMING ALGORITHM, in fact, it
  // never stops unless there is an error, in that case it returns false.
/*
  
private:
 
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  FPGroup theGroup; // original presentation
  File file;        // the list of alternative presentations
*/
};

