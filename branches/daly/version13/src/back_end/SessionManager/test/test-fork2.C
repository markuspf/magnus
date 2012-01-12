// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test of fork technology.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//

#include <sys/wait.h>
#include <stdio.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <vfork.h>
#include "global.h"
#include "Timer.h"


void spendTime( char ch )
{
  for( int k = 0; k < 10; ++k ) {
    
    // spend some time
    for( int i = 0; i < 500; ++i )
      for( int j = 0; j < 1000; ++j )
	;

    // and do something
    cout << ch << endl;
  }
}



int main ( )
{

  pid_t pid;

  if ( (pid = fork()) == 0 ) {
   cout << "Child: I want to sleep." << endl;
   sleep(5);
   cout << "Child: I'm finishing." << endl;
   _exit(0);
  }

  cout << "Parent: I want to sleep." << endl;

  sleep(10); 

  cout << "Parent: I'm killing zombie." << endl;
  
  int status;
  wait(&status);

  cout << "Parent: I want to sleep again." << endl;

  sleep(10); 
  
  cout << "Parent: I'm finishing now." << endl;
}



