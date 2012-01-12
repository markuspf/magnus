// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test of the class DataSet
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include "Integer.h"
#include "Set.h"
#include "DataSet.h"

int main ( )
{
  SetOf<double> S(5);
  DataSet DS(S);
  cout << DS.mean();
}


