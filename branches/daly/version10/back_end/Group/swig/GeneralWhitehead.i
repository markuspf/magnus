%module Magnus

%{
#include "GeneralWhitehead.h"
%}

class GeneralWhitehead 
{

public:

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  GeneralWhitehead( const FreeGroup& F, bool keepDetails = false );

  ~GeneralWhitehead( );
      

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void startComputation( const VectorOf<Word>& );
  // Start the computation. 
  // You shouldn't call this more than ones.

  %name(startComputationFromWord) void startComputation( const Word& );
  // The same as previsious one, but for one word only.
             
  bool continueComputation( );
  // Advance the computation. 
  // You have to call startComputation() before.
  // This function returns true if the computation is done.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Status Queries:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool done( ) const;
  // true only when the computation is finished.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // You can call all these functions iff the computation is finished
  // ( when the done() functions returns true ).

  bool extendsToFreeBasis( );
  Map getAutomorphism( );

  Chars getFileName( );
  // Returns name of the file with computation details
  
/*
private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const int timerValue = 1000;

  bool bDone;
  bool bStart;
  bool doesExtend;
  bool itIsInterrupted;
  Timer timer;

  const FreeGroup theGroup;
  int numberOfGenerators;
  VectorOf<Word> images;
  int *numOfAuto;
  Map automorphism;
  VectorOf<Word> theVector;
  int theVectorLen;
  int theVectorCommonLength;
  int saveN;

  File* file;

  bool hasOneWordOnly;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private methods:                                                    //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  GeneralWhitehead( const GeneralWhitehead& );
  // It is hidden, not implemented.

  GeneralWhitehead& operator = ( const GeneralWhitehead& );
  // It is hidden, not implemented.

  Word makeWhiteheadAutomorpism( Generator, int, Generator ) const;

  void prepareAutomorphism( Map& automorphism ) const;

  void setResult( bool DoesExtend );

  void finishComputation( bool DoesExtend );

  int commonLength( const VectorOf<Word>& );
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Debugging stuff:                                                    //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef debug_GeneralWhitehead

  int numOfShortenings;

  friend int main( );

#endif
*/
};
