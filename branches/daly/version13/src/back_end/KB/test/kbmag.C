#include "MagnusHome.h"
#include "BlackBox.h"
#include "Map.h"
#include "FPGroup.h"
#include "KBmagPackage.h"
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


  Chars startDir(MagnusHome::magnusHome()+"/back_end/KB/test/bin");

  int pid;

  if( !(pid = fork()) ) {

    KBmagPackage KBM( G.namesOfGenerators(), G.getRelators() );
    
    Trichotomy answer = KBM.autgroup();
    
    char sPid[100];
    sprintf(sPid, "%d", getpid());
    
    ofstream out(startDir+"/tmp/results."+sPid,
		 ios::out | ios::trunc | ios::bin );
    if( !out ) 
      error("Cannot open an output file.");
    
    out < answer;

    if( answer == yes )
      out < KBM.differenceMachine(1) < KBM.wordAcceptor();
    
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
  
  Trichotomy answer(dontknow);
  in > answer;

  if( answer == yes )
    cout << "The group is automatic.";
  else
    if( answer == no )
      cout << "The algorithm fails to prove the group automatic";
    else
      cout << "The algorithm is unable to complete "
	"(probably through lack of memory).";

  cout << endl << endl;

  if( answer != yes ) return 0;

  DiffMachine DM;
  GroupDFSA WA;
  in > DM > WA;
  
  cout << "The size of the language: " << WA.sizeLanguage() << endl << endl;
  
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
    DM.rewrite( temp );


    cout << "temp = ";
    G.printWord(cout, temp);
    cout << endl << endl;

    
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
