%module Magnus

%{
#include "PresentationsOfSubgroup.h"

%}

class PresentationsOfSubgroup 
{
 public:
  
  PresentationsOfSubgroup( const Subgroup& );

  FPGroup makePresentation( File &tmpOutput );
  // Computes a presentation for this subgroup if it's possible using
  // the method of progressive approximations.As a result returns 
  // finitely presented group.
  
  FPGroup makePresentationTC( const PermutationRepresentation &pr,
			      File &tmpOutput );
  // Computes a presentation for this subgroup if it's possible using
  // Todd-Coxeter procedure.As a result returns finitely presented group.

  FPGroup makePurePresentationTC( const PermutationRepresentation &pr );
 
  Word rewriteWord( const PermutationRepresentation& pr , const Word& w );
  // Rewrites a word in terms of Schreier generators
/*
 private:
  
  Subgroup H;
  FPGroup G;
  VectorOf<Word> theGenerators;
*/
};





