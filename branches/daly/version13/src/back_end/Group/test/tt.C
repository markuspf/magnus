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


#include "TTP.h"
#include "Map.h"

#define debug_ttp

int main ( )
{
  FPGroup G;
  cout << endl << "Enter a finitely presented group: ";
  cin >> G;
  cout << endl;

  TTP ttp(G);
  cout << "File name: " << ttp.getFileName() << endl;
  ttp.run();
}





