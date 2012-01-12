// Contents: 
//
// Principal Authors: Dmitry Bormotov
//


#include "Pair.h"
#include "Set.h"
#include "Word.h"

bool isPrime( int n )
{
  for( int i = 2; i < n; ++i )
    if (n % i == 0 ) 
      return false;
  return true;
}

bool uniqueFactors(int n)
{
  int count = 0;
  for( int i = 2; i < n; ++i )
    if( n%i == 0 ) {
      int j = n/i;
      if( j < i ) break;
      if( (i+j)%2 != 0  && !isPrime(i+j-2) ) {
	if( ++count > 1 ) return false;
      }
    } 
  return true;
}

void main( )
{
  SetOf<Pair> S,S2;
  for( int a = 2; a <= 100; ++a )
    for( int b = a; b <= 100; ++b )
      if( (a+b)%2 != 0  && !isPrime(a+b-2) )
	if( uniqueFactors(a*b) ) S |= Pair(a,b);
  
  SetIterator<Pair> I(S);
  cout << "W3 is a set of cardinality: " << S.cardinality() << endl 
       << "{" << endl;

  for( ;!I.done(); I.next())
    cout << Pair(I.value());
  cout << endl << "}" << endl;

  for( int i = 4; i <= 200; ++i ) {
    int count = 0;
    for( I.reset(); !I.done(); I.next())
      if( I.value().a + I.value().b == i ) ++count;
    if( count == 1 )
      for( I.reset(); !I.done(); I.next())
	if( I.value().a + I.value().b == i ) S2 |= I.value();
  }

  SetIterator<Pair> I2(S2);
  cout << "W4 is a set of cardinality: " << S2.cardinality() << endl 
       << "{" << endl;
  for( ;!I2.done(); I2.next())
    cout << Pair(I2.value());
  cout << endl << "}" << endl;
}


