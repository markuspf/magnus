// Contents: Computer Project 2
//           Compute the number of primes in [1..N]
//
// Principal Authors: Dmitry Bormotov
//


#include "global.h"
#include "Random.h"

const int bound = 10000;

bool isPrime( int n )
{
  for( int i = 2; i < n; ++i )
    if (n % i == 0 ) 
      return false;
  return true;
}

void main( )
{
  int count = 0;
  UniformRandom r;

  for( int i = 1; i <= bound; ++i )
    if( isPrime( (int)(r.rand() * bound) ) )
      ++count;
  cout << count;
}


