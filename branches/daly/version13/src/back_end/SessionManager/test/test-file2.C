// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//

#include <stdio.h>
#include "global.h"
#include "MagnusHome.h"
//#include "Chars.h"

int main ( )
{
  const int bufSize = 100;
  {
   
    char s[bufSize];
    for( int i = 0; i < bufSize; ++i )
      s[i] = 0;

    FILE *out;
    if( (out = fopen(MagnusHome::magnusHome()+"/out", "w")) == NULL ) 
      error("Cannot open an output file.");
    
    ostrstream ostr(s, bufSize-1);
    
    //write
    bool one = true;
    bool two = false;
    ostr << one << " "<< two << " ";

    fputs(s, out);
    fclose(out);
  }

  {
    char s[bufSize];
    for( int i = 0; i < bufSize; ++i )
      s[i] = 0;
    
    FILE *out;
    if( (out = fopen(MagnusHome::magnusHome()+"/out", "r")) == NULL ) 
      error("Cannot open an output file.");
    
    fgets(s, bufSize-1, out);
    istrstream istr(s, bufSize-1);

    //read
    bool one, two;
    istr >> one >> two;
    
    fclose(out);
    
    cout << "one: ";
    if( one )
      cout << "true";
    else
     cout << "false";
    cout << endl;

    cout << "two: ";
    if( two )
      cout << "true";
    else
     cout << "false";
    cout << endl;
       
  }
}



