// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents:
//
// Function(s) to compute greatest common divisor.
//
// Principal Authors: Eugene Paderin, Dmitry Pechkin
//
// Status: draft
//
// Revision History:
//
// Discussion:
//
// Bugs:
//
//
// Special Notes:
//
//

#ifndef _GCD_H_
#define _GCD_H_

int GCD(int a, int b);
// the greatest common divisor

int GCD(int a, int b, int& p, int& q);
//for given a, b finds the greatest common divisor and p, q 
//such that p*a + q*b = gcd(a,b).


int LCM(int a, int b);
// the least common multiplier


#endif

