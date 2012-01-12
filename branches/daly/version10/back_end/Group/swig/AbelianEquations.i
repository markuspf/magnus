%module Magnus

%{
#include "AbelianEquations.h"
%}

class AbelianEquationsSolver {

public:
  
  AbelianEquationsSolver( );

  %name(AbelianEquationsSolverFromGroup) AbelianEquationsSolver( const AbelianGroup& a , 
			  const VectorOf<Word>& v ,
			  int numOfVar );
  
 
  void findSolutions( File& file , bool out = true );
  
  VectorOf<Word> getBasicSolutions() const;
  
  VectorOf< VectorOf< VectorOf<int> > > getTorsionPart() const;
  
  VectorOf< VectorOf< VectorOf<int> > > getParametrizedPart() const;
  
  int getSystemRank() const;
  
  int getNumberOfParams() const;
  
  bool solIsAllGroup( ) const;
  
  bool haveSolutions() const;
/*  
private:
  
  bool root( Word& , int ) const;
  
  void makeSystem();
  
  void printRawSystem( File& ) const;
  void printSystem( File& ) const;
  
  AbelianGroup rawA;
  AbelianGroup A;
  VectorOf<Word> rawSystem;
  VectorOf<Word> system;
  VectorOf<Word> b;
  int numberOfVariables;
  int sysRank;
  int haveSol; // -1 - system have no solutions
               //  0 - system have the group A as a set of solutions
               //  1 - the set of solutions isn't equal to A
  
  VectorOf<Word> x;
  VectorOf< VectorOf< VectorOf<int> > > torsion;
  VectorOf< VectorOf< VectorOf<int> > > params;  
*/
};





