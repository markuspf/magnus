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
#include <signal.h>
#include <unistd.h>
#include <fstream.h>
#include "global.h"
#include "MagnusHome.h"
#include "Vector.h"
#include "Word.h"
#include "Set.h"
 
Word initWord(const Generator* p, int len) {
  VectorOf<Generator> v(len);
  for( int i = 0; i < len; i ++ ) v[i] = *p++;
  return Word(v);
}


int main ( )
{
  Chars startDir(MagnusHome::magnusHome()+"/back_end/Elt/test/bin");

  char sPid[100];
  sprintf(sPid, "%d", getpid());

  ofstream out(startDir+"/tmp/results."+sPid,
	       ios::out | ios::trunc | ios::bin );
  if( !out ) 
    error("Cannot open an output file.");
  
  //write
  bool one = true;
  bool two = false;
  Chars s1("Hello");
  Chars s2(" Dima");
  Chars s3(" Bormotov !");
  VectorOf<Chars> v(3);
  v[0] = s1;
  v[1] = s2;
  v[2] = s3;

  const Generator gl3[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1};
  const Generator gl4[] = { 2, 2, 2, 2, 2, 2, 2, 2, 2 };
  const Generator gl5[] = { 1, 2, 3, -1, 2, -3, -3 };
  
  Word r3, r4, r5;
  r3 = initWord(gl3, 9);
  r4 = initWord(gl4, 9);
  r5 = initWord(gl5, 7);
  SetOf<Word> rels3;
  rels3 |= r3;
  rels3 |= r4;
  rels3 |= r5;

  out < one < two < s1 < s2 < s3 < v < r3 < r4 < r5 < rels3;
  out.flush();

  return 0;
}
