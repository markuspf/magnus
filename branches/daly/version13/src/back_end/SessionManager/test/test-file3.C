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


int main ( )
{
  const int bufSize = 100;
  Chars fnResults(MagnusHome::magnusHome()+"/results");
  
  if( fork() == 0 ) {

    cout << "FN: " << fnResults << endl;
    ofstream out(fnResults, ios::out | ios::trunc );
    if( !out ) 
      error("Cannot open an output file.");
    
    //write
    bool one = true;
    bool two = false;
/*    out << one << " "
	<< two << " ";
	*/
    out << one
	<< two;

    cout << "Child: Finish." << endl;
    exit(0);
  }
  else {

    sleep(2);

    ifstream in(fnResults, ios::in | ios::nocreate );
    if( !in )
      error("Cannot open an input file.");

    //read
    bool one = true, two = true;
    in >> one >> two;

    cout << "one: " << one << endl;
    cout << "two: " << two << endl;
  }
}



