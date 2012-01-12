// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of classes RingEltParser, MatrixParser,
//           MonomialParser, PolynomialParser.
//
// Principal Author: Dmitry Bormotov
//
// Status: In development
//
// Usage:
//
// Discussion:
//
// Revision History:
//


#include "RingParser.h"


//------------------------------ RingEltParser ------------------------------//


template <class R>
void RingEltParser<R>::parseWhiteSpace( )
{
  char ch;

  // Flush whitespace & comments.
  do {
    if ( peekCh() == '#' )
      while ( peekCh() != '\n' ) {
	istr.get();
      }
    if ( ( ch = peekCh() ) == ' ' || ch == '\t' || ch == '\n' ) {
      ch = ' ';
      getCh();
    }
  } while ( ch == ' ' );
}


Integer RingEltParser<Integer>::parseRingElt( Chars& errMesg )
{
  Integer result;
  
  if ( curToken !=  INT ) {
    parseError("expected an integer here");
    errMesg = parseErrorMessage;
    return result;
  }
  
  result = tokenInt;
//  getToken();
  return result;
}


Rational RingEltParser<Rational>::parseRingElt( Chars& errMesg )
{
  Integer numerator, denominator;
  
  if ( curToken !=  INT ) {
    parseError("expected an integer here");
    errMesg = parseErrorMessage;
    return Rational();
  }

  numerator = tokenInt;
/*
  getToken();

  if ( curToken ==  COMMA || curToken ==  RSQUARE ) {
    denominator = 1;
    return Rational(numerator, denominator);
  }

  if ( curToken !=  BAD || getCh() != '/' ) {
    parseError("expected a '/' here");
    errMesg = parseErrorMessage;
    return Rational();
  }
*/
  
  parseWhiteSpace();
  char ch = peekCh();
  if( ch == ',' || ch == ']' ) {
    denominator = 1;
//    getCh();
    return Rational(numerator, denominator);
  }

  if ( getCh() != '/' ) {
    parseError("expected a '/' here");
    errMesg = parseErrorMessage;
    return Rational();
  }
  
  getToken();

  if ( curToken !=  INT ) {
    parseError("expected an integer here");
    errMesg = parseErrorMessage;
    return Rational();
  }

  denominator = tokenInt;
//  getToken();

  if( denominator == 0 ) {
    parseError("invalid rational: zero denominator here");
    errMesg = parseErrorMessage;
    return Rational();
   }
  
  return Rational(numerator, denominator);
}


//----------------------------- MatrixParser --------------------------------//


template <class R>
VectorOf<R> MatrixParser<R>::parseMatrixRow( Chars& errMesg )
{
  VectorOf<R> result; 

  if ( curToken == INIT ) getToken();

  if ( curToken !=  LSQUARE ) {
    parseError("expected a '[' here");
    errMesg = parseErrorMessage;
    return result;
  }

  getToken();

  if ( curToken !=  INT ) {
    parseError("expected an integer here");
    errMesg = parseErrorMessage;
    return result;
  }

  while( true ) {

    R elem = parseRingElt(errMesg);
    getToken();
    
    if( errMesg.length() > 0 )
      return result;
    result.append(elem);
    
    if ( curToken ==  RSQUARE ) {
      getToken();
      return result;
    }
    
    if ( curToken !=  COMMA ) {
      parseError("expected a ',' here");
      errMesg = parseErrorMessage;
      return result;
    }
    
    getToken();
  }
}


template <class R>
void MatrixParser<R>::deleteMatrix( R ***M, int MSize )
{
  for( int i = 0; i < MSize; i++ )
    delete [] M[i];
  delete [] M; 
}


template <class R>
bool MatrixParser<R>::parseMatrix( Chars& errMesg, R ***M, int& MSize )
{
  int numOfRows = 0;
  
  if ( curToken == INIT ) getToken();
  
  if ( curToken !=  LSQUARE ) {
    parseError("expected a '[' here");
    errMesg = parseErrorMessage;
    return false;
  }

  getToken();
  bool bFirstIteration = true;
  
  while( true ) {
    VectorOf<R> row = parseMatrixRow(errMesg);
    if( errMesg.length() > 0 ) {
      if( !bFirstIteration )
	deleteMatrix(M, MSize);
      return false;
    }
    
    if( bFirstIteration ) {
      bFirstIteration = false;
      MSize = row.length();
      *M = new (R*)[MSize];
      for( int i = 0; i < MSize; i++ ) {
	(*M)[i] = new R[MSize];
      }
    }
    else
      if( row.length() != MSize ) {
	parseError("incorrect length of this row");
	errMesg = parseErrorMessage;
	deleteMatrix(M, MSize);
	return false;
      }

    if( numOfRows >= MSize) {
      parseError("too many rows here");
      errMesg = parseErrorMessage;
      deleteMatrix(M, MSize);
      return false;
    }
    
    for( int i = 0; i < MSize; i++ ) 
      (*M)[numOfRows][i] = row[i];

    numOfRows++;

    if( curToken == RSQUARE )
      if( numOfRows == MSize )
	return true;
      else {
	parseError("not enough rows here");
	errMesg = parseErrorMessage;
	deleteMatrix(M, MSize);
	return false;
      }
    else
      if( curToken == COMMA )
	getToken();
      else {
	parseError("expected ',' or ']' here");
	errMesg = parseErrorMessage;
	deleteMatrix(M, MSize);
	return false;
      }
  }
}

//--------------------------- MonomialParser -------------------------------//


template <class R>
Monomial<R> MonomialParser<R>::getMonomial( Chars& errMesg,
					    bool& isLastMonomial )
{
  int powersOfVars[maxNumberOfVariables];
  R coef = 1;
  int numOfVars = 0;
  isLastMonomial = false;

  for( int i = 0; i < maxNumberOfVariables; ++i )
    powersOfVars[i] = 0;

  parseWhiteSpace();
  char ch = peekCh();

  if( ch == '-' ) {
    getCh();
    coef = -1;
  }
  else
    if( ch == '+' )
      getCh();
  
  parseWhiteSpace();
  ch = tolower(peekCh());

  if ( ch >= '0' && ch <= '9' ) {
    getToken();

    coef *= parseRingElt(errMesg);
    if ( errMesg.length() > 0 ) {
      return Monomial<R>( );
    }

    parseWhiteSpace();

    if( ( ch = tolower(peekCh()) ) != 'x' ) {
      if( ch == ',')
	getToken();
      return Monomial<R>(coef);
    }
  }

  if( ch != 'x' ) {
    parseError("expected a 'x' here");
    errMesg = parseErrorMessage;
    return Monomial<R>( );
  }
  
  while( true ) {
    
    int index, power;
    getCh();
    getToken();
    
    if( curToken != INT || (index = tokenInt) <= 0 ) {
      parseError("expected a positive integer here");
      errMesg = parseErrorMessage;
      return Monomial<R>( );
    }
    
    parseWhiteSpace();
    ch = tolower(peekCh());

    if( ch == '^' ) {
      getCh();
      getToken();
      
      if( curToken !=  INT || (power = tokenInt) <= 0 ) {
	parseError("expected a positive integer here");
	errMesg = parseErrorMessage;
	return Monomial<R>( );
      }

      parseWhiteSpace();
      ch = peekCh();
    }
    else
      power = 1;
    
    powersOfVars[index-1] += power;
    if( numOfVars < index )
      numOfVars = index;

    if( ch != 'x' ) {
      if( coef == 0 )
	numOfVars = 0;
      if( ch == ',' ) {
	getToken();
	isLastMonomial = true;
      }
      return Monomial<R>(coef, numOfVars, powersOfVars);
    }
  }
}

template <class R>
Monomial<R> MonomialParser<R>::parseMonomial( Chars& errMesg )
{
  bool isLastMonomial;
  return getMonomial(errMesg, isLastMonomial);
}



//--------------------------- PolynomialParser ------------------------------//


template <class R>
Polynomial<R> PolynomialParser<R>::parsePolynomial( Chars& errMesg )
{
  Monomial<R> M;
  int koef = 1;
  bool isLastMonomial;
  Polynomial<R> result;

  while( true ) {
    M = getMonomial(errMesg, isLastMonomial);
    if ( errMesg.length() > 0 ) {
      return Polynomial<R>( );
    }

    result.insert(M);
    
    if( isLastMonomial )
      return result;
    
    char ch = peekCh();
    if( ch != '+' && ch != '-' ) {
      parseError("expected a sign here");
      errMesg = parseErrorMessage;
      return Polynomial<R>( );
    }
  }
}



template class MatrixParser<Integer>;
template class MatrixParser<Rational>;
template class MonomialParser<Integer>;
template class MonomialParser<Rational>;
template class PolynomialParser<Integer>;
template class PolynomialParser<Rational>;
