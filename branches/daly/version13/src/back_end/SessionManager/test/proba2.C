// Copyright (C) 1999 The New York Group Theory Cooperative
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
#include <signal.h>
#include <unistd.h>
#include <fstream.h>
#include "global.h"
#include "MagnusHome.h"
#include "Vector.h"
#include "Word.h"
#include "Set.h"
#include "AbelianInvariants.h"
#include "SMFPGroup.h"


int main ( )
{
  FPGroup F;
  SMFPGroup G(F,"Hello");
  AbelInvariantsProb CM(G);
  AbelianInvariantsARCer A(CM);
  bool b = A.takeControl();
  if( b )
    cout << "True";
  else
    cout << "False";
  cout << endl;
}


