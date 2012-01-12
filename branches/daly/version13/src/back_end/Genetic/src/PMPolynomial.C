// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of class PMPolynomial
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "PMPolynomial.h"


//#define DEBUG_PMPOLYNOMIAL


// ---------------------------- PMPolynomial ------------------------------- //


PM* PMPolynomial::mutate( )
{
  Polynomial<Int2> res;

  do {

    // make a random elementary auto
    
    UniformRandom& r = cfg.rng();
    int var = min(cfg.getNumOfVars() - 1, int(r.rand() * cfg.getNumOfVars()));
    
    Polynomial<Int2> rp = randomPolynomial(cfg.getMaxNumOfMonoms(), var);
    
    //cout << "rp: " << endl << rp << endl;
    
    int *p = new int[var+1];
    for( int i = 0; i < var+1; ++i )
      p[i] = 0;
    p[var] = 1;
    
    rp = rp + Monomial<Int2>(1,var+1,p);
    delete [] p;
    
    //cout << "rp2: " << endl << rp << endl;
    
    res = Polynomial<Int2>();
    
    // apply the auto
    
    PolynomialIterator<Int2> PI(poly);
    for( ; !PI.done(); PI.next() ) {
      Monomial<Int2> M = PI.getValue();
      int pow;
      if( M.getNumberOfVariables() >= var+1 
	  && (pow = M.getPowerOfVariable(var)) != 0 ) {
	
	M.setPowerOfVariable(var,0);
	if( var+1 == M.getNumberOfVariables() ) {
	  int *p = new int[var];
	  for( int i = 0; i < var; ++i )
	    p[i] = M.getPowerOfVariable(i);
	  M = Monomial<Int2>(M.getCoefficient(), var, p );
	  delete [] p;
	}
	
	Polynomial<Int2> t(1);
	for( int j = 0; j < pow; ++j )
	  t = t * rp;
	res = res + (t * M); 
      }
      else 
	res = res + M;
    }

#ifdef DEBUG_PMPOLYNOMIAL
    cout << "Original polynomial: " << endl << poly << endl;
    cout << "Replacing variable x" << var + 1 << " with polynomial" 
	 << endl << rp << endl;
    cout << "The result is " << endl << res << endl << endl;
#endif
  } while( res.degree() > 10 );

  return new PMPolynomial(cfg, res);
}


Monomial<Int2> PMPolynomial::randomMonomial( int maxCoef, int NumOfVars, 
					     int maxPower, int absentVar )
{
  Monomial<Int2> M;

  //do { 
    UniformRandom& r = cfg.rng();;
    int coef = 1 + int(r.rand() * (maxCoef-1));
    if( r.rand() < 0.5 ) 
      coef = -coef;
    
    int numOfVars = NumOfVars;
    if( numOfVars == 0 || coef == 0 ) 
      return Monomial<Int2>((Int2)(coef));
    
    int* p = new int[numOfVars];
    
    for( int i = 0; i < numOfVars; ++i ) {
      p[i] = int(r.rand() * maxPower);
    }
    p[absentVar] = 0;
    
    while( numOfVars > 0 && p[numOfVars-1] == 0 ) 
      --numOfVars;
    if( numOfVars == 0 )
      return Monomial<Int2>((Int2)(coef));
    
    M = Monomial<Int2>(coef, numOfVars, p);
    delete [] p;
    //} while( Polynomial<Int2>(M).degree() > 10 );
  
  return M;
}


Polynomial<Int2> PMPolynomial::randomPolynomial( int maxNumOfMonoms, 
						 int absentVar )
{
  UniformRandom& r = cfg.rng();
  int num =  1 + int(r.rand() * (cfg.getMaxNumOfMonoms()-1));
  Polynomial<Int2> p;
  
  for( int i = 0; i < num; ++i )
    p += randomMonomial( cfg.getMaxCoef(), cfg.getNumOfVars(), 
			 cfg.getMaxPower(), absentVar );

  return p;
}
