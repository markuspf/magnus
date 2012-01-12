// Contents: Computer Project 1
//           Compute the number of even integers in [1..N]
//
// Principal Authors: Dmitry Bormotov
//


#include "global.h"
#include "Random.h"

void main( )
{
  int count = 0;
  UniformRandom r;
  const int bound = 10000;

  for( int i = 1; i <= bound; ++i )
    if( ( (int)(r.rand() * bound) ) % 2 == 0 )
      ++count;
  cout << count;
}


