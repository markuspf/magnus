// Contents: Definitions of classes UniformRandom and NormalRandom,
//           for generating random numbers.
//
// Principal Author: Dmitry Bormotov
//
// Status: Useable.
//
// Revision History:
//
// * 09/97 Dmitry B. added function long TimeSeed( ).
//         From now on UniformRandom and NormalRandom use it as a
//         default seed value. 
//

#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <iostream.h>
#include "Timer.h"

extern "C" double sqrt(double);
extern "C" double log(double);


inline long TimeSeed() {
  timeval tp;
  gettimeofday(&tp,0);
  return tp.tv_sec;
}


class UniformRandom {

// An object in this class generates uniformly random numbers of type
// int and float.

public:

  UniformRandom(int seed = TimeSeed()) { reseed(seed); }

  void reseed(int seed);
  // In particular, calling this with the same seed as passed to the
  // constructor starts the random sequence over.

  float rand( );
  // Returns uniformly random floats in [0.0, 1.0].

  int rand(int lower, int upper);
  // Returns uniformly random ints in [lower, upper]

private:

  const int   M1  = 259200;
  const int   IA1 = 7141;
  const int   IC1 = 54773;
  const float RM1 = (1.0/M1);
  const int   M2  = 134456;
  const int   IA2 = 8121;
  const int   IC2 = 28411;
  const float RM2 = (1.0/M2);
  const int   M3  = 243000;
  const int   IA3 = 4561;
  const int   IC3 = 51349;

  long ix1, ix2, ix3;
  float r[98];
};



class NormalRandom {

// An object in this class generates normally distributed random numbers
// of type int and float.

public:

  NormalRandom(int seed = TimeSeed()) : uniformDeviates(seed), iset(0) { }

  void reseed(int seed) { uniformDeviates.reseed(seed); }
  // In particular, calling this with the same seed as passed to the
  // constructor starts the random sequence over.

  float rand( );
  // Returns normally distributed random floats with mean 0 and variance 1.

  int rand(int mean, int stddev);
  // Returns normally distributed random ints with mean mean and standard
  // deviation stddev. For example, rand(100, 20) returns ints, roughly 68%
  // of which fall within 20 of 100, and 95% within 40 of 100.
  // Beware that rand(100, 20) could return a negative int, though it is
  // unlikely. You may want something like
  //   do { i = G.rand(100, 20); } while ( i < 1 );

private:

  UniformRandom uniformDeviates;
  int iset;
  float gset;
};

#endif
