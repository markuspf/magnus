#include "APwithOneRelator.h"
#include "NormalRandomWord.h"
#include "Automorphism.h"


void main() 
{
  APwithOneRelator AP;
  
  cout << "Enter an amalgamated product of free group with one relator "
    "(e.g < <a,b>*<c,d>; a = c>): "
    << endl;

  Chars errMesg = cin >> AP; 
  if( errMesg.length() > 0 ) {
    cout << errMesg << endl;
    return 1;
  }
  cout << "You entered: " << AP << endl << endl;
  
  FPGroup G( AP.namesOfGenerators(), AP.getRelators() );

  cout << "FP Group: " << G << endl << endl;
}

