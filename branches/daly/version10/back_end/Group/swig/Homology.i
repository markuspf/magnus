///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works

%module Magnus

%{
  #include "Homology.h"
%}

class Homology {

public:

  //////////////////////////////////////////////////////////////
  //                                                          //
  // Constructors:                                            //
  //                                                          //
  //////////////////////////////////////////////////////////////

//  Homology(const KBMachine M, int start_dim, int end_dim);
  // M specifies the group of which we want the homology, and
  // 1 < start_dim <= end_dim are the dimensions we want.

  ~Homology( );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool workOnNextGroup( );
  // See usage comments

  //////////////////////////////////////////////////////////////////////
  //                                                                  //
  // Accessors:                                                       //
  //                                                                  //
  //////////////////////////////////////////////////////////////////////

  int getTorsionFreeRank( ) const;
  // See usage comments

  VectorOf<Integer> getTorsionInvariants( ) const;
  // See usage comments

  // Could also allow queries on the status of the current computation.
  %addmethods{
     Homology(){
     }
  }
/*
private:

  //////////////////////////////////////////////////////////////////////
  //                                                                  //
  // Data Members:                                                    //
  //                                                                  //
  //////////////////////////////////////////////////////////////////////

  char tempFileName[100];
  // The executable chom wants its input from a file; this is the name.
  
  BlackBox* chom;
  // Wraps the chom executable.

  enum State { READING, REDUCING, GOT_ONE };
  State myState;

  int dimensionToDo;
  // Calls to workOnNextGroup() work on this dimension.

  int lastDimension;
  // A check against runaway calls to workOnNextGroup().

  SmithNormalForm* SNF;
  // The current smith normal form computation, or NULL if none.

  bool booted;
  // true iff we have read and reduced the first, `bootstrap' matrix.

  int previousTorsionFreeRank;
  int columns;

  //////////////////////////////////////////////////////////////////////
  //                                                                  //
  // Private Members:                                                 //
  //                                                                  //
  //////////////////////////////////////////////////////////////////////

  Integer** readMatrix(int& rows, int& cols) const;
  // Read the next matrix from *chom's output.
*/
};
