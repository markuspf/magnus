//   Copyright (C) 1994 The New York Group Theory Cooperative
//   See magnus/doc/COPYRIGHT for the full notice.
 
// Contents: 
//
// Principal Author:
//
// Status: Useable.
//
// Revision History:
//
 
#ifndef _Proba_h_
#define _Proba_h_

#include "iostream.h"

class foo {

public:

  foo();
  ~foo();

  virtual int ()foo(int(foo::* pmf)(int),int i)
  { 
    return i;
  }

  int g(int);
  virtual int h(int);
};



#endif
