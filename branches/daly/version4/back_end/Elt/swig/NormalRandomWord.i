
%module Magnus

%{
#include "NormalRandomWord.h"
%}

class NormalRandomWord {

public:

  NormalRandomWord(int numberOfGenerators, int meanLength, int lengthStddev,
						 int lengthSeed, int generatorSeed);

  Word word( );
/*
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const NormalRandomWord& nRW )
  {
    ostr < nRW.numGens < nRW.mean < nRW.stddev < nRW.G < nRW.L;
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, NormalRandomWord& nRW )
  {
    istr > nRW.numGens > nRW.mean > nRW.stddev > nRW.G > nRW.L;
    return istr;
  }

private:

  int numGens, mean, stddev;

  UniformRandom G;
  // For choosing letters in the word.

  NormalRandom L;
  // For choosing the length of the word.

*/
};

