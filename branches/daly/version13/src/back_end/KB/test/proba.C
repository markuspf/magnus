#include "Map.h"
#include "FPGroup.h"
#include "RKBPackage.h"
#include <sys/wait.h>

main()
{
  cout << "Enter an FP group: ";
  
  FPGroup G;
  Chars errMsg = cin >> G;
  if ( errMsg.length() > 0 ) {
    cout << errMsg << endl << endl;
    return 1;
  }

  cout << endl;

  RKBPackage RKBP(G.namesOfGenerators(), G.getRelators());
  
  int maxRelLen = 0;
  SetIterator<Word> I( G.getRelators() );
  while ( !I.done() ) {
    Word w = I.value();
    int l = w.length();
    if ( l > maxRelLen ) maxRelLen = l;
    I.next();
  }

  int rkbp_par = 2 * maxRelLen;
//  RKBP.startKB();

  Chars startDir(MagnusHome::magnusHome()+"/back_end/KB/test/bin");

  int pid;

  do {

//    if( !(pid = fork()) ) {
      
      RKBP.runKB(rkbp_par, -1, rkbp_par, rkbp_par);

//      cout << "runKB() finished" << endl;
/*      
      char sPid[100];
      sprintf(sPid, "%d", getpid());
      
      ofstream out(startDir+"/tmp/results."+sPid,
		   ios::out | ios::trunc | ios::bin );
      if( !out ) 
	error("Cannot open an output file.");
      
      out < RKBP;
      out.flush();
      _exit(0);
    }

    
    wait(NULL);
    
    char sPid[100];
    sprintf(sPid, "%d", pid);
    
    ifstream in(startDir+"/tmp/results."+sPid,
		ios::in | ios::nocreate | ios::bin );
    if( !in )
      error("Cannot open an input file.");
    
    in > RKBP;
 */
    rkbp_par += (rkbp_par >> 1);
   
  } while( !RKBP.isProvedConfluent() );
  
  KBMachine M = RKBP.KnuthBendixMachine();


  ofstream out(startDir+"/tmp/M1", ios::out | ios::trunc | ios::bin );
  if( !out ) 
    error("Cannot open an output file.");
  
  out < M;
  out.flush();

  
  ifstream in(startDir+"/tmp/M1", ios::in | ios::nocreate | ios::bin );
  if( !in )
    error("Cannot open an input file.");
  
  in > M;


  ofstream out2(startDir+"/tmp/M2", ios::out | ios::trunc | ios::bin );
  if( !out2 ) 
    error("Cannot open an output file.");
  
  out2 < M;
  out2.flush();

  
  bool quit = false;
  while( !quit ) {
    
    cout << "Enter a word: ";
    Word w = G.readWord(cin, errMsg);
    if ( errMsg.length() > 0 ) {
      cout << errMsg << endl << endl;
      return 1;
    }
    
    cout << endl;
    
    Word temp = w;
    M.rewrite( temp );
    
    cout << "The word  ";
    G.printWord(cout, w);
    cout << "  is";
    if( temp.length() )
      cout << " not";
    cout << " trivial." << endl << endl;
    
    char reply;
    cout << "Repeat (y/n)? ";
    do {
      cin >> reply;
      reply = tolower(reply);
    } while( reply != 'y' && reply != 'n' );
    if( reply == 'n' ) quit = true;
    cout << endl;
  }
  
}
