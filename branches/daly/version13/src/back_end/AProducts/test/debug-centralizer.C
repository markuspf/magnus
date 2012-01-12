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

	cout << "Testing the amalgamated product of free groups: " << endl;

	while(1)  {
		cout << "Enter your word (or 1 to quit): " << endl;
		Word w = AP.readWord(cin, errMesg);
		if( errMesg.length() > 0 )  {
			cout << endl << errMesg << endl;
			return 1;
		}

		cout << "You entered: ";
		AP.printWord(cout, w);
		cout << endl;

		if( w.length() == 0 ) {
			cout << "empty word entered. Exiting..\n";
			break;
		}

		VectorOf<Word> centralizer = AP.centralizerOf(w);
		cout << " centralizer of ";
		AP.printWord(cout, w);
		cout << " is ";
		AP.printVectorOfWords(cout, centralizer);
		cout << endl;

		// check:
		for(int j = 0; j < centralizer.length(); ++j) {
			if( !AP.commute(w, (Word)centralizer[j]) ) {
				cout << "Error: the word does not commute with ";
				AP.printWord(cout, centralizer[j]);
				cout << endl;
			}
			cout << endl << endl;
		}
	}

}

