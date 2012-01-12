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

const int MAXLEV = 20;	// maximum recursion level - should be sufficient always

//-----------------------------------------------------------------------------//
// local function prototype
//-----------------------------------------------------------------------------//


//-----------------------------------------------------------------------------//

double AdaptiveSimpson (double a, double b, 
			double (*funct)(double x), 
			double relerr);

//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//

double AdaptiveSimpsonInt (double a, double b, int m,
			double (*funct)(double x,int n), 
			double relerr);
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//

double AdaptiveSimpsonIntInt (double a, double b, int m1, int m2,
			double (*funct)(double x,int n1,int n2), 
			double relerr);

//-----------------------------------------------------------------------------//
