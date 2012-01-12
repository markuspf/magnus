///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
  #include "GACPforORGSolver.h"
%}

class GAConjProblemForORGroupSolver
{
 public:

  /////////////////////////////////////////////////////////////////
  //                                                             //
  // Constructors:                                               //
  //                                                             //
  /////////////////////////////////////////////////////////////////

  GAConjProblemForORGroupSolver( const OneRelatorGroup& group , const Word& W1 , const Word& W2 , bool createFile = true , bool cp = true );

  ~GAConjProblemForORGroupSolver( );

//  GAConjProblemForORGroupSolver operator = ( const GAConjProblemForORGroupSolver& solver ) ;
  // hidden
/*
 protected:

  GAConjProblemForORGroupSolver( const OneRelatorGroup& group , const Word& W1 , const Word& W2 , File* f );
  // this constructor used for conjectures only
*/
 public:

  

  //////////////////////////////////////////////////////////////////
  //                                                              //
  // Accessors:                                                   //
  //                                                              //
  //////////////////////////////////////////////////////////////////

  bool isConj( );

  int getNumberOfIterations( ) const;
  // get total number of iterations done by Solver::isConj and internal conjectures

  Chars getFileName( ) const ;


  //////////////////////////////////////////////////////////////////
  //                                                              //
  //  Internal functions                                          //
  //                                                              //
  //////////////////////////////////////////////////////////////////

 public:
/*
  static int rnd1( int max );  
  static int roulette( double d1 , double d2 );
  %name(rouletteFromInt) static int roulette( int num , double* d);
  
  static Word greedyReduce( const OneRelatorGroup& group , const Word& word );
  static bool oneGreedyReduce( const OneRelatorGroup& group , Word& w );
  static void insert( Word& dst , const Word& src , int pos);
*/
  int rnd1( int max );  
  int roulette( double d1 , double d2 );
  %name(rouletteFromInt) int roulette( int num , double* d);
  
  Word greedyReduce( const OneRelatorGroup& group , const Word& word );
  bool oneGreedyReduce( const OneRelatorGroup& group , Word& w );
  void insert( Word& dst , const Word& src , int pos);

/*
 protected:


  void checkImprovementTime( );

  int reproduction( );
  bool tournament( GACPforORGSolverGene& gene );
  int selectGene( ) const;


  // I invoke this function when get more shorter words, in the constructor 
  // and if there is so long time since last improvement
  // It resets pair of words
  void toStart( const Word& W1 , const Word& W2 );

 protected:

  //////////////////////////////////////////////////////////////////
  //                                                              //
  //  Data members                                                //
  //                                                              //
  //////////////////////////////////////////////////////////////////

  static double prob[2][3];
  //two sets of probability of mutation, permutation and crossover

  bool conjProblem;
  // it it sets then it solves conjugacy problem
  // else solves word problem

  int lastImprovement;
  //
 
  int fitnessRate;

  int theIter1, theIter2;
  // number of iterations

  double bestFit;
  // best fitness value have ever obtained 

  File* file;
  bool deleteFile;
  // session file

  //////////////////////////////////////////////////////////////////
  //                                                              //
  //  Checked words                                               //
  //                                                              //
  //////////////////////////////////////////////////////////////////

  AssociationsOf< Word , int > checkedWords;


  //////////////////////////////////////////////////////////////////
  //                                                              //
  //  Genes                                                       //
  //                                                              //
  //////////////////////////////////////////////////////////////////

  int numGenes;
  GACPforORGSolverGene** genes;
  GACPforORGSolverGene *newGene[2];


  //////////////////////////////////////////////////////////////////
  //                                                              //
  //  Algebraic objects                                           //
  //                                                              //
  //////////////////////////////////////////////////////////////////

  OneRelatorGroup theGroup;
  Word theWord1;
  Word theWord2;
  
  static const int NOCONJ;
*/
};
