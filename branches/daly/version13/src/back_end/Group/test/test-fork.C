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


#include <signal.h>
#include <unistd.h>
#include "global.h"
#include "Timer.h"


void spendTime( char ch )
{
  for( int k = 0; k < 50; ++k ) {
    
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
  int pid;
  Timer timer(0);

  if ( ( pid = fork() ) == 0 ) {
    cout << "Child: I am starting..." << endl;
    spendTime('C');
    cout << "Child: I am finishing..." << endl;
    exit(1);
  }
  
  if( pid < 0 ) {
    cout << "fork failed." << endl;
  }  
  
  cout << "Parent: my child's pid is" << pid << endl;
  kill(pid, SIGSTOP);
  
  while( kill(pid, 0) == 0 ) {
    timer.reset(1000);
    kill(pid, SIGCONT);
    while( !timer.expired() ) ;
    kill(pid, SIGSTOP);
    cout << "Parent: Interruption." << endl;
    cout << "Parent: I will sleep for 5 seconds." << endl;
    kill(pid,0);
    sleep(5);
    cout << "Parent: Continue." << endl;
  }

  cout << "Parent: I'm finishing now." << endl;
}



