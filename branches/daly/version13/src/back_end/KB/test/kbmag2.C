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

  KBmagPackage KBM( G.namesOfGenerators(), G.getRelators() );
  Trichotomy answer = KBM.autgroup();
  
  if( answer == yes )
    cout << "The group is automatic.";
  else
    if( answer == no )
      cout << "The algorithm fails to prove the group automatic";
    else
      cout << "The algorithm is unable to complete "
	"(probably through lack of memory).";
  
  cout << endl << endl;

  if( answer == yes ) {
    cout << "Word Acceptor: " << endl << endl; 
    KBM.wordAcceptor().printOn(cout);
    cout << endl << endl; 
    cout << "Difference Machine 1: " << endl << endl;
    KBM.differenceMachine(1).printOn(cout);
    cout << endl << endl;
    cout << "Difference Machine 2: " << endl << endl;
    KBM.differenceMachine(2).printOn();
    cout << endl << endl;
    cout << "General Multiplier: " << endl << endl;
    KBM.generalMultiplier().printOn(cout);
    cout << endl << endl;
  }
  /*
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
  */
}
