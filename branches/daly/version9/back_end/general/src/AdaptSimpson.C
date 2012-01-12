/*-----------------------------------------------------------------------------*\
| Matpack integration package - adaptive simpson                adaptsimpson.cc |
|                                                                               |
| MatPack Library Release 1.0                                                   |
| Copyright (C) 1991-1996 by Berndt M. Gammel                                   |
|                                                                               |
| Permission to  use, copy, and  distribute  Matpack  in  its entirety  and its |
| documentation  for non-commercial purpose and  without fee is hereby granted, |
| provided that this license information and copyright notice appear unmodified |
| in all copies.  This software is provided 'as is'  without express or implied |
| warranty.  In no event will the author be held liable for any damages arising |
| from the use of this software.						|
| Note that distributing Matpack 'bundled' in with any product is considered to |
| be a 'commercial purpose'.							|
| The software may be modified for your own purposes, but modified versions may |
| not be distributed without prior consent of the author.			|
|                                                                               |
| Read the  COPYRIGHT and  README files in this distribution about registration	|
| and installation of Matpack.							|
|                                                                               |
\*-----------------------------------------------------------------------------*/
#include <math.h>
#include "AdaptSimpson.h"

//-----------------------------------------------------------------------------//
// double AdaptiveSimpson (double a, double b, 
//                         double (*f)(double x), 
//                         double relerr);
//-----------------------------------------------------------------------------//
// 
// DESCRIPTION:
//
//   This routine integrates the function f in the intervall [a,b] to a relative
//   accuracy of relerr using an adaptive Simpson algorithm. This is a general
//   purpose algorithm which can be applied to a variety of problems. It is
//   applicable also for functions that have singularities or which are rapidly
//   changing. But it is not very fast. The result becomes exact for polynomials
//   of degree three.
//
//   The error boundary should be considered carefully, because two 
//   approximations are compared. If there is a strong cancelation of terms 
//   of the integral, for instance if the sign of the function is oscillating 
//   rapidely, then the relative error might be larger than expected.
//
//   In principal the adaptive quadrature should be used only in the case
//   of complicated integrals (for instance with singlarities or fast changing
//   functions) or as a test of the results of faster specialized integration
//   methods (Gauss integration or other non-adaptive methods).
//
//   The function  is based on algorithm 145 of  W. M. McKeeman
//   from the "Collected Algorithm from ACM". 
//
// IMPLEMENTATION NOTES:
//
//   Recursion is explicitly used, since it is quite effective
//   in the C language. A non-recursive Fortran version can be found in
//   ACM algorithm 182.  
//  
//   C++ version by Berndt M. Gammel, last change Nov 13, 1996.
//
//-----------------------------------------------------------------------------//


//-----------------------------------------------------------------------------//
int max(int x, int y)
{
   if(x>=y)
      return x;
   else
      return y;
}

//-----------------------------------------------------------------------------//

static double RecursiveSimpson (double a, double da, 
				double fa, double fm, double fb,
				double area, double est, double relerr,
				double (*funct)(double x),
				int &level, int &levmax)
{
    const double norm = 0.588, 		// heuristic constant = 1/sqrt(3)
                 one_third = 1.0 / 3.0,
                 one_18th  = 1.0 / 18.0;

    double dx  = one_third * da,
           ddx = one_18th * da,
           relerrs = relerr * norm,
           x1 = a + dx,
           x2 = x1 + dx,
           f1 = 4 * funct(a + 0.5 * dx),
           f2 = funct(x1),
           f3 = funct(x2),
           f4 = 4 * funct(a + 2.5 * dx),
           est1 = (fa + f1 + f2) * ddx,
           est2 = (f2 + fm + f3) * ddx,
           est3 = (f3 + f4 + fb) * ddx,
           absarea = area - fabs(est) + fabs(est1) + fabs(est2) + fabs(est3),
           sum = est1 + est2 + est3;

    level++;
    levmax = max(level,levmax);
    if ( ((fabs(est - sum) > relerr*absarea) || (est == 1.0)) && (level < MAXLEV) )
      sum = RecursiveSimpson( a,dx,fa,f1,f2,absarea,est1,relerrs,funct,level,levmax)
          + RecursiveSimpson(x1,dx,f2,fm,f3,absarea,est2,relerrs,funct,level,levmax)
          + RecursiveSimpson(x2,dx,f3,f4,fb,absarea,est3,relerrs,funct,level,levmax);
    level--;
    return sum;
}

//-----------------------------------------------------------------------------//

double AdaptiveSimpson (double a, double b, 
			double (*funct)(double x), 
			double relerr)
{
    int levmax = 1, level = 1;
    
    return RecursiveSimpson(a, 
			    b - a, 
			    funct(a),
			    funct(0.5*(a + b)) * 4, 
			    funct(b), 
			    1.0, 1.0, fabs(relerr), funct, level, levmax);
}

//-----------------------------------------------------------------------------//

static double RecursiveSimpsonInt (double a, double da, 
				double fa, double fm, double fb, int m,
				double area, double est, double relerr,
				double (*funct)(double x,int n),
				int &level, int &levmax)
{
    const double norm = 0.588, 		// heuristic constant = 1/sqrt(3)
                 one_third = 1.0 / 3.0,
                 one_18th  = 1.0 / 18.0;

    double dx  = one_third * da,
           ddx = one_18th * da,
           relerrs = relerr * norm,
           x1 = a + dx,
           x2 = x1 + dx,
           f1 = 4 * funct(a + 0.5 * dx,m),
           f2 = funct(x1,m),
           f3 = funct(x2,m),
           f4 = 4 * funct(a + 2.5 * dx,m),
           est1 = (fa + f1 + f2) * ddx,
           est2 = (f2 + fm + f3) * ddx,
           est3 = (f3 + f4 + fb) * ddx,
           absarea = area - fabs(est) + fabs(est1) + fabs(est2) + fabs(est3),
           sum = est1 + est2 + est3;

    level++;
    levmax = max(level,levmax);
    if ( ((fabs(est - sum) > relerr*absarea) || (est == 1.0)) && (level < MAXLEV) )
      sum = RecursiveSimpsonInt( a,dx,fa,f1,f2,m,absarea,est1,relerrs,funct,level,levmax)
          + RecursiveSimpsonInt(x1,dx,f2,fm,f3,m,absarea,est2,relerrs,funct,level,levmax)
          + RecursiveSimpsonInt(x2,dx,f3,f4,fb,m,absarea,est3,relerrs,funct,level,levmax);
    level--;
    return sum;
}

//-----------------------------------------------------------------------------//


double AdaptiveSimpsonInt (double a, double b, int m,
			double (*funct)(double x,int n), 
			double relerr){
    int levmax = 1, level = 1;
    
    return RecursiveSimpsonInt(a, 
			    b - a, 
			    funct(a,m),
			    funct(0.5*(a + b),m) * 4, 
			    funct(b,m), m,
			    1.0, 1.0, fabs(relerr), funct, level, levmax);

}

//-----------------------------------------------------------------------------//

static double RecursiveSimpsonIntInt (double a, double da, 
				double fa, double fm, double fb, int m1, int m2,
				double area, double est, double relerr,
				double (*funct)(double x,int n1,int n2),
				int &level, int &levmax)
{
    const double norm = 0.588, 		// heuristic constant = 1/sqrt(3)
                 one_third = 1.0 / 3.0,
                 one_18th  = 1.0 / 18.0;

    double dx  = one_third * da,
           ddx = one_18th * da,
           relerrs = relerr * norm,
           x1 = a + dx,
           x2 = x1 + dx,
           f1 = 4 * funct(a + 0.5 * dx,m1,m2),
           f2 = funct(x1,m1,m2),
           f3 = funct(x2,m1,m2),
           f4 = 4 * funct(a + 2.5 * dx,m1,m2),
           est1 = (fa + f1 + f2) * ddx,
           est2 = (f2 + fm + f3) * ddx,
           est3 = (f3 + f4 + fb) * ddx,
           absarea = area - fabs(est) + fabs(est1) + fabs(est2) + fabs(est3),
           sum = est1 + est2 + est3;

    level++;
    levmax = max(level,levmax);
    if ( ((fabs(est - sum) > relerr*absarea) || (est == 1.0)) && (level < MAXLEV) )
      sum = RecursiveSimpsonIntInt( a,dx,fa,f1,f2,m1,m2,absarea,est1,relerrs,funct,level,levmax)
          + RecursiveSimpsonIntInt(x1,dx,f2,fm,f3,m1,m2,absarea,est2,relerrs,funct,level,levmax)
          + RecursiveSimpsonIntInt(x2,dx,f3,f4,fb,m1,m2,absarea,est3,relerrs,funct,level,levmax);
    level--;
    return sum;
}
//-----------------------------------------------------------------------------//
double AdaptiveSimpsonIntInt (double a, double b, int m1, int m2,
			double (*funct)(double x,int n1,int n2), 
			double relerr){
    int levmax = 1, level = 1;
    
    return RecursiveSimpsonIntInt(a, 
			    b - a, 
			    funct(a,m1,m2),
			    funct(0.5*(a + b),m1,m2) * 4, 
			    funct(b,m1,m2), m1,m2,
			    1.0, 1.0, fabs(relerr), funct, level, levmax);

}
