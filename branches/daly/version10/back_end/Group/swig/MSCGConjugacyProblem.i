%module Magnus

%{
#include "MSCGConjugacyProblem.h"
%}
class MSCGConjugacyProblem {


public:

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // No default constructor

  // Copy constructor provided by compiler (does logical deep copy).

  MSCGConjugacyProblem ( const MSCGroup& G, const Word& u, const Word& v );
  // To construct a conjugacy problem class with given MSCGroup.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void startComputation( );
    // Start the computation.      
  // You shouldn't call this more than one time.
  
  bool continueComputation( );
  // Advance the computation of conjugacy problem.
  // You have to call startComputation() before.
  // This function returns true if the computation is done.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Status Queries:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool done( ) const;
  // Is the conjugacy problem done ?

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // You can call all these functions iff the computation is finished
  // ( when the done() functions returns true ).

  Trichotomy answer( ) const;
  // Are the words conjugate ? 

  Word conjugator( ) const;
/* 
private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private Functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  MSCGConjugacyProblem( const MSCGConjugacyProblem& );
  // It is hidden, not implemented.

  MSCGConjugacyProblem& operator = ( const MSCGConjugacyProblem& );
  // It is hidden, not implemented.

  void setCPResult( Trichotomy result );
  // Set result of conjugacy problem and reset status variables.

  void finishCP( Trichotomy result );
  // Finish conjugacy problem ( delete all objects and set result).
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const MSCGroup& MSCG;    // The MSC group in which we solve a 
                           // conjugacy problem
  bool doneStatus;         // TRUE if the computation is finished
  bool startStatus;        // TRUE if the computation is started
  Trichotomy areConjugate; // the result of conjugacy problem
  Word theConjugator;      // the conjugator if the words are conjugate
  int maxLen;              // maximal length of relators which we 
                           // need use for generating pieces
  Word UConjugator;
  Word VConjugator;

  // This variables contain information for iterations of conjugacy problem

  enum stateType{CYCLE_BY_RELATORS, CYCLE_BY_PIECES, CYCLE_BY_RELATORS2};

  stateType state;         // In what kind of cycle we are now ?
  bool firstPart;          // TRUE if we are in first part of algorithm: 
                           //   for C'(1/8) condition 
  Word U;                  // fisrt argument for conjugacy problem
  Word V;                  // second argument for conjugacy problem
  int ULen;                // length of U
  int VLen;                // length of V
  SetOf<Word> cycV;        // the set of cyclic permutations of V

  SymmetricRelators *shortRelators;      
  // The set of short relators.
  // Only this relators would be able to shorten given word

  SymmetricRelatorsIterator *shortIter;  // symmetric iterators over the
  SymmetricRelatorsIterator *shortIter2; // set of relators

  Word relator;        // current relator 
  int relatorLen;      // its length
  int pieceLen;        // tne length of current piece of current relator
  
  Word relatorOne;     // the first current relator
  int relatorOneLen;   // its length
  int pieceOneLen;     // the length of fisrt piece of relatorOne
  Word pieceOne;       // the fisrt piece of relatorOne 
  Word relatorTwo;     // the second current relator
  int relatorTwoLen;   // its length

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Debugging stuff:                                                    //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  friend int main(...);

#endif
*/
};
