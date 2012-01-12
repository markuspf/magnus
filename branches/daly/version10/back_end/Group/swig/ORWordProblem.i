%module Magnus

%{
#include "ORWordProblem.h"
%}

class ORWordProblem {

public:

  //////////////////////////////////////////////////////////////
  //                                                          //
  // Constructors:                                            //
  //                                                          //
  //////////////////////////////////////////////////////////////

  ORWordProblem(const Word& relator);

  ~ORWordProblem( );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void findAnswer(const Word& testWord);
  // This will not return until it has the answer.

  //////////////////////////////////////////////////////////////////////
  //                                                                  //
  // Accessors:                                                       //
  //                                                                  //
  //////////////////////////////////////////////////////////////////////

  bool goodStatus( );

  bool isTrivial( );
/*
private:

  //////////////////////////////////////////////////////////////////////
  //                                                                  //
  // Data Members:                                                    //
  //                                                                  //
  //////////////////////////////////////////////////////////////////////

  BlackBox* orwp;
  // Wraps the orwp executable.

  const Word theRelator;

  bool status;
  // True until something goes wrong, then we give up altogether.

  bool result;
  // The last answer computed.

  //////////////////////////////////////////////////////////////////////
  //                                                                  //
  // Private Members:                                                 //
  //                                                                  //
  //////////////////////////////////////////////////////////////////////

  void printWord(const Word& w, ostream& ostr);
  // Output w in Dion's format.
*/
};
