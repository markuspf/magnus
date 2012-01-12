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


#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fstream.h>
#include "global.h"
#include "MagnusHome.h"
#include "Vector.h"
#include "Word.h"
#include "Set.h"


int main ( )
{
  const Chars startDir(
	        MagnusHome::magnusHome()+"/back_end/SessionManager/test/bin");
  int pid;
  
  if( ( pid = fork() ) == 0 ) {

    execl(startDir+"/child", "child", 0);

    error("Cannot execute my child.");
    _exit(1);
  }

  if( pid < 0 )
    error("Fork failed.");
  
  int status;
  wait(&status);

  char sPid[100];
  sprintf(sPid, "%d", pid);

  ifstream in(startDir+"/tmp/results."+sPid,
	      ios::in | ios::nocreate | ios::bin );
  if( !in )
    error("Cannot open an input file.");
  
  //read
  bool one = true, two = true;
  VectorOf<Chars> v;
  SetOf<Word> rels3;
  Word r3;
  Chars s1(" ");
  Chars s2(" ");
  Chars s3(" ");

  in >> one >> two;
  s1.read(in);
  s2.read(in);
  s3.read(in);
  v.read(in);
  r3.read(in);
  rels3.read(in);
  
  cout << "one: " << one << endl;
  cout << "two: " << two << endl;
  cout << "s1: " << s1 << endl;
  cout << "s2: " << s2 << endl;
  cout << "s3: " << s3 << endl;
  cout << "v: " << v << endl;
  cout << "r3: " << r3 << endl;
  cout << "rels3: " << rels3 << endl;
}



