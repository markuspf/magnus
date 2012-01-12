%module Magnus

%{
#include "GraphConjugacyProblem.h"

%}



class GraphConjugacyProblem 
{

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  GraphConjugacyProblem( const FPGroup&, const Word& u, const Word& v );

  ~GraphConjugacyProblem( ) ;

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void startComputation( );  
  // Start the problem: are u and v conjugate ?      
  // You shouldn't call this more than one time.
         
  %name(continueComputationFromSubgroupGraph) void continueComputation( const SubgroupGraph& theGraph );
  // Advance the computation of conjugacy problem.

  void continueComputation( );
  // Advance the computation of conjugacy problem.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Status Queries:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool done( ) const ;
  // Is the conjugacy problem done ?
  // If it is done then u and v are conjugate.

  bool theNewGraphIsNeeded( ) const ;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // You can call all these functions iff the computation is finished
  // ( when the done() functions returns true ).


  Word getConjugator( ) ;

/*

private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
    
  int numberOfGenerators;
  int maxGeneratorLength;
  Word theConjugator;
  const FPGroup theGroup;
  Word UConjugator;
  Word VConjugator;

  Word U;                  // fisrt argument for conjugacy problem
  Word V;                  // second argument for conjugacy problem
  bool bStart;             // TRUE if the computation is started
  bool bDone;              // TRUE if the computation is finished

  DoubleCosetGraph *DCG;
  DCGVertexIterator *I;
  Timer *timer;
  static const int timerValue = 1000;

  
  // The data which is necessary to divide findConjugator() on arcs.

  bool isInterrupted; 
  DoubleWayElt *way;
  int saveWayIndex;
  int saveLabel;
  DCGState saveLeftState;
  DCGState saveRightState;
  int *leftMarks;
  int *rightMarks;

 
  //////////////////////////////////////////////////////////////
  //                                                          //
  // Private functions:                                       //
  //                                                          //
  //////////////////////////////////////////////////////////////

  GraphConjugacyProblem( const GraphConjugacyProblem& );
  // It is hidden, not implemented.

  GraphConjugacyProblem& operator = ( const GraphConjugacyProblem& );
  // It is hidden, not implemented.

  void finishComputation( )
  {
    bDone = true;
    // bStart = false;
    theConjugator = theGroup.shortenByRelators(theConjugator);
    theConjugator = VConjugator.inverse() * theConjugator * UConjugator;
    theConjugator = theConjugator.inverse();
    if( isInterrupted )
      finishInterruption();
  }

  bool findConjugator( const DCGState& state1, DCGLabelType label1,
		       const DCGState& state2, DCGLabelType label2, 
		       int length, Word& conjugator);

  bool theConjugatorIsFound( const DCGState& state1, 
                             const DCGState& state2 ) const;

  void mark( int *, const DCGState&, int *, const DCGState&, int );

  void clear( int *, const DCGState&, int *, const DCGState& );

  void getMarks( const int *, const DCGState&, const int *, const DCGState&,
		int&, int& ) const;

  bool weHaveTheSameCycles( int *, const DCGState&, 
			    int *, const DCGState& ) const;

  void finishInterruption( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  friend int main(...);

#endif
*/
};


