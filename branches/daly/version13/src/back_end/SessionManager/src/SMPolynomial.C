// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMPolynomial
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "SMPolynomial.h"


//---------------------------- SMPolynomial ---------------------------------//


SMPolynomial::SMPolynomial( const Polynomial<Integer>& P, 
			    const Chars heritage )
  : AlgebraicObject( heritage ),
    thePolynomial( P )
{ }

void SMPolynomial::printProperties(ostream& ostr) const
{
}

void SMPolynomial::viewStructure(ostream& ostr) const
{
  ostr
    << "viewBuilder objectView viewParameters {viewID "
    << oid()
    << " heritage {"
    << heritage()
    << "}}";
}


void SMPolynomial::printDefinition(ostream& ostr) const
{
  ostr << thePolynomial;
}




