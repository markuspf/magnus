// Contents: Computer Project 15
//           
//
// Principal Authors: Dmitry Bormotov
//


#include "global.h"
#include "Random.h"
#include "Integer.h"

void main( )
{
  Integer f = 1;
  for( int i = 1; i <= 100; ++i )
    f *= i;
  cout << f << endl;
}


