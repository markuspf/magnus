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


//#include <values.h>
#include "global.h"
#include "Queue.h"
#include "Word.h"

Word initWord(const Generator* p, int len) {
  VectorOf<Generator> v(len);
  for( int i = 0; i < len; i ++ ) v[i] = *p++;
  return Word(v);
}

void main ( )
{
  QueueOf<int> q;
  q.fpush(5);
}






