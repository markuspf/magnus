// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of DataSet class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "Rational.h"
#include "Set.h"
#include "DataSet.h"


// ------------------------------- DataSet --------------------------------- //


double DataSet::mean( ) const 
{
  double sum = 0;
  SetIterator<double> I(theSet);
  for( ; !I.done(); I.next() )
    sum += I.value();
  return sum / cardinality();
}

double DataSet::deviation( ) const
{
  double m = mean();
  double sum = 0;
  SetIterator<double> I(theSet);
  for( ; !I.done(); I.next() )
    sum += (I.value() - m) * (I.value() - m);
  return sqrt( sum / (cardinality() - 1) );
}

void DataSet::eatWhite( istream& istr, int& pos )
{
  while( !istr.eof() ) {
    char ch = istr.peek();
    if( ch == ' ' || ch == '\t' || ch == '\n' ) {
      ++pos;
      istr.get();
    }
    else
      break;
  }
} 

/* under development

void DataSet::readFrom( istream& istr, Chars& errMsg )
{
  // We expect a list of numbers separated by commas and enclosed in
  // braces.

  int pos = 0;
  char c;
  double d;
  
  // Expecting '{'
  
  eatWhite();
  if( istr.eof() || istr.peek() != '{' ) {
    errMsg = Chars(pos) + "{ '\\{' expected.}";
    return;
  }
  
  istr.get(); ++pos; eatWhite();
  
  // Expecting number or '}'
  
  if( istr.eof() ) {
    errMsg = Chars(pos) + "{ '\\}' expected.}";
    return;
  }

  c = istr.peek();
  if( c == '}' ) {
    errMsg = "";
    // **** 
    return;
  }
 
  istr >> d;

  if( !istr ) {
    errMsg = Chars(pos) + "{Number expected.}";
    return;
  }
 
  S |= d;
  
  

}

*/


// temporary simple version

void DataSet::readFrom( istream& istr, Chars& errMsg )
{
  // We expect a list of numbers separated by commas and enclosed in
  // braces.

  int pos = 0;
  char c;
  double d;
  SetOf<double> S; 
 
  // Expecting '{'
  
  eatWhite(istr,pos);
  if( istr.eof() || istr.peek() != '{' ) {
    errMsg = "Error.";
    return;
  }
  
  istr.get(); eatWhite(istr,pos);
  
  while( !istr.eof() ) {
    
    c = istr.peek();

    if ( c == '}' ) {
      theSet = S;
      errMsg = "";
      return;
    }

    else if ( c == ',' ) {
      istr.get();
    }

    else {
      istr >> d;
      if ( !istr ) {
	errMsg = "Error.";
	return;
      }
      S |= d;
   }
    
    eatWhite(istr,pos);
  }
}





