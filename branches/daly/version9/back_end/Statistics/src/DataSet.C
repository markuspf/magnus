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

#include "DataSet.h"

// ------------------------------- DataSet --------------------------------- //


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


// temporary simple version

void DataSet::readFrom( istream& istr, Chars& errMsg )
{
  // We expect a list of numbers separated by commas and enclosed in
  // braces. { {...} {.....} .... }

  int pos = 0;
  char c;
  double d;
  
  eatWhite(istr,pos);
  if( istr.eof() || istr.peek() != '{' ) {
    errMsg = " { expected ";
    return;
  }
  
  istr.get(); eatWhite(istr,pos);
  
  while( !istr.eof() && istr.peek() != '}'  ) {
    VectorOf<double> S; 

    if( istr.eof() || istr.peek() != '{' ) {
       errMsg = " { expected ";
       return;
    }

    istr.get(); eatWhite(istr,pos);
    while( !istr.eof()){
        c = istr.peek();
        if ( c == '}' ) {
           if(S.length()==0){
              errMsg = "no elements!";
              return;
	   }else{ 
              theSets.append(S);
              istr.get(); eatWhite(istr,pos);
               errMsg = "";
               break;
           }
	}else if ( c == ',' ) {
            istr.get();
	}else {
            istr >> d;
            if ( !istr ) {
        	     errMsg = "data is Error.";
	             return;
	    }
            S.append(d);
	}
    
        eatWhite(istr,pos);
    }
  }
  if(getDimension()==0){
              errMsg = "no dataset defined!";
              return;
  }
}

