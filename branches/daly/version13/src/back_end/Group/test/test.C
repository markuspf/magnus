// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: 
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include "stdio.h"
#include "TTP.h"
#include "Map.h"
#include "iostream.h"
#include "fstream.h"

#define debug_ttp

int main ( )
{
  cout << endl << "Enter a filename: ";
  Chars s;
  cin >> s;


/*
  ifstream f( (const char*)s, ios::in | ios::nocreate );
  
  if( !f )
    error( "Cannot open a file." );

  char c;

  while( true )
    if( f.get(c) ) cout.put(c);
*/

  FILE* f;
  f = fopen( (const char*)s, "r" );
  while( true ) { fseek(f, 0, SEEK_END); cout << ftell(f) << endl; }
}
