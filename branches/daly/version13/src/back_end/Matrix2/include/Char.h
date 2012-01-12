// Temporary implemenation for debugging.


#include "global.h"

class Char {

public:


  Char( char c = ' ') : ch( c ) { }

  int compare( const Char& c ) const
  {
    if( ch == c.ch )
      return 0;
    else
      if( ch < c.ch )
	return -1;
      else
	return 1;
  }

  friend ostream& operator << ( ostream& ostr, const Char& c )
  {
    ostr << c.ch;
    return ostr;
  }

  
private:

  char ch;

};

