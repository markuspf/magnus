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


#include "Word.h"
#include "DataSet.h"

int main ( )
{
  Word w; //@db temporary, to shut up the bad linker

  cout << "Enter your data set, enclosed in braces: ";
  DataSet S;
  cin >> S;
  cout << endl << endl;

  /*
  SetOf<double> set;
  for( int i = 1; i <= 3; ++i )
    set |= double(i);
  DataSet S(set);
  */

  cout << "Your data set: " << endl << S << endl << endl;
  cout << "The mean: " << S.mean() << endl;
  cout << "The standard deviation: " << S.deviation() << endl;
}


