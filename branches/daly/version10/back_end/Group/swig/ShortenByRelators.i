%module Magnus

%{
#include "ShortenByRelators.h"
%}

class ShortenByRelators {

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  // No default constructor
  // Copy constructor provided by compiler (does logical deep copy).
  
  ShortenByRelators ( const SymmetricRelators& symmetricRelators );
  // To construct a ShortenByRelators class of given symmetric relators.

 // %name(ShortenByRelatorsFromSet) ShortenByRelators ( const SetOf<Word>& ); 
  // Special hack to forbid construct ShortenByRelatos of 
  // non symmetrised set of relators.

  ~ShortenByRelators( );
    
  ///////////////////////////////////////////////////////
  //                                                   //
  // Accessors:                                        //
  //                                                   //
  ///////////////////////////////////////////////////////

  Word getShortenWord( const Word& w ) const;  
  // Return shorten form of word w.
/*  
private:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Data members:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  Word* sortRelators;    // relators in sorted order, used for optimization 
  int sortRelatorsLen;   // the number of sorted relators
*/
};

