%module Magnus

%{
#include "RandomMSCGroup.h"
%}

class RandomMSCGroup {

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  RandomMSCGroup ( 
    int numOfGenerators, int generatorsSeed,
    int meanLengthOfRelators, int lengthOfRelatorsStddev, int lengthOfRelatorsSeed, 
    int meanNumberOfRelators, int numberOfRelatorsStddev, int numberOfRelatorsSeed
  ) ;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors:                                       //
  //                                                   //
  ///////////////////////////////////////////////////////

  FPGroup getRandomMSCGroup();
  // This is non const functions.
/*
private:
 

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Data members:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  int numOfGens;
  int gensSeed;
  int meanLenOfRels;
  int lenOfRelsStddev;
  int lenOfRelsSeed;
  int meanNumOfRels;
  int numOfRelsStddev;
  NormalRandom numOfRelsSeed;
*/
};
