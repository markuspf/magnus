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
  FILE *file;
  Chars s(MagnusHome::magnusHome()+"/in");
  cout << "s = " << s << endl;
  if( (file = fopen(s, "w")) == NULL ) 
    error("Cannot open first file.");
  fputs("Hello!\0", file);
  fclose(file);

  if( (file = fopen(s, "r")) == NULL ) 
    error("Cannot open second file.");
  char t[100];
  for( int i = 0; i < 100; ++i )
    t[i] = 0;
  fgets(t, 99, file);
  cout << "Received: " << t << endl;
  fclose(file);
}



