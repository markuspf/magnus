
%module Magnus

%{
#include "RandomNumbers.h"
%}

class UniformRandom {

// An object in this class generates uniformly random numbers of type
// int and float.

public:

//  UniformRandom(int seed = TimeSeed());
  UniformRandom(int seed);
  void reseed(int seed);
  // In particular, calling this with the same seed as passed to the
  // constructor starts the random sequence over.

  float rand( );
  // Returns uniformly random floats in [0.0, 1.0].

  %name(randFromInt) int rand(int lower, int upper);
  // Returns uniformly random ints in [lower, upper]

/*
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const UniformRandom& unRand )
  {
    ostr < unRand.ix1 < unRand.ix2 < unRand.ix3;
    for (int i = 0; i<98; i++) { ostr << unRand.r[i] << ' ';}// There is no > and < 
    return ostr;                                             // operators for float in IPC.h
  }
  
  friend istream& operator > ( istream& istr, UniformRandom& unRand)
  {
    istr > unRand.ix1 > unRand.ix1 > unRand.ix2 > unRand.ix3;
    for (int i = 0; i<98; i++) { istr >> unRand.r[i];}
     return istr;
  }

private:

  static const int   M1  = 259200;
  static const int   IA1 = 7141;
  static const int   IC1 = 54773;
  static const float RM1 = (1.0/M1);
  static const int   M2  = 134456;
  static const int   IA2 = 8121;
  static const int   IC2 = 28411;
  static const float RM2 = (1.0/M2);
  static const int   M3  = 243000;
  static const int   IA3 = 4561;
  static const int   IC3 = 51349;

  long ix1, ix2, ix3;
  float r[98];
*/
};



class NormalRandom {

// An object in this class generates normally distributed random numbers
// of type int and float.

public:

//  NormalRandom(int seed = TimeSeed());
  NormalRandom(int seed);
  void reseed(int seed);
  // In particular, calling this with the same seed as passed to the
  // constructor starts the random sequence over.

  float rand( );
  // Returns normally distributed random floats with mean 0 and variance 1.

  %name(randFromInt) int rand(int mean, int stddev);
  // Returns normally distributed random ints with mean mean and standard
  // deviation stddev. For example, rand(100, 20) returns ints, roughly 68%
  // of which fall within 20 of 100, and 95% within 40 of 100.
  // Beware that rand(100, 20) could return a negative int, though it is
  // unlikely. You may want something like
  //   do { i = G.rand(100, 20); } while ( i < 1 );
/*
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const NormalRandom& noRand )
  {
    ostr < noRand.uniformDeviates < noRand.iset;
    ostr << noRand.gset << ' ';
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, NormalRandom& noRand)
  {
    istr > noRand.uniformDeviates > noRand.iset;
    istr >> noRand.gset;
    return istr;
  }

private:

  UniformRandom uniformDeviates;
  int iset;
  float gset;
*/
};
