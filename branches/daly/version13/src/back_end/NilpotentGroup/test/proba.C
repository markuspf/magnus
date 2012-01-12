// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: 
//
// Principal Authors: Alexei Myasnikov.
//

#include "global.h"
#include "conversions.h"
#include "NilpotentGroup.h"
#include "SGOfNilpotentGroup.h"
#include "Map.h"
#include "Polynomial.h"

void main( )
{
  Polynomial<Integer> P;
  Map M;
 
  FPGroup G;
  cout << "Enter nilpotentcy class first then group ";
  int NilpotentcyClass;
  cin >> NilpotentcyClass;
  Chars errMsg = cin >> G;
  if (errMsg.length()>0) 
    return 1;
 
  NilpotentGroup ng(G.namesOfGenerators(),NilpotentcyClass,
		    makeVectorOf(G.getRelators()));
  
  cout << "Your group: " << ng << endl;

  ng.initialize();
}
  



