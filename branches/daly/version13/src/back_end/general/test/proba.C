// Copyright (C) 1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: 
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision /History:
//

#include "/usr/local/include/python2.1/Python.h"
#include "Chars.h"

void main ( )
{
  cout << "Enter command: ";
  char s[1001];
  cin.get(s,1000,'\n');
  cout << endl;
  cout << "Your command: " << s << endl;
  cout << "Running Python ..." << endl;

  Py_Initialize();
  PyRun_SimpleString(s);
  Py_Finalize();
}








