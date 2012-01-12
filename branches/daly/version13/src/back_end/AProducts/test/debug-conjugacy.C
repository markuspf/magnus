#include "APwithOneRelator.h"
#include "NormalRandomWord.h"
#include "Automorphism.h"

ostream& operator<< (ostream& o, const Trichotomy& t) 
{
	if( t == yes ) 
		o << "yes";
	else if( t == no ) 
		o << "no";
	else
		o << "dontknow";
	return o;
}

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

	cout << "Testing conjugacy problem: " << endl;

	while(1)  {
		cout << "Enter your words (or 1 to quit): " << endl;
		Word w = AP.readWord(cin, errMesg);
		if( errMesg.length() > 0 )  {
			cout << endl << errMesg << endl;
			return 1;
		}

		if( w.length() == 0 ) {
			cout << "1st word is empty. Exiting..\n";
			break;
		}

		Word u = AP.readWord(cin, errMesg);
		if( errMesg.length() > 0 ) {
			cout << endl << errMesg << endl;
			return 1;
		}

		// displays entered words.
		cout << "You entered: w = ";
		AP.printWord(cout, w);
		cout << ", u = ";
		AP.printWord(cout, u);
		cout << endl;

		// runs conjugacy problem.
		Word conjugator;
		Trichotomy answer = AP.conjugacyProblem(w,u,conjugator);
		cout << "the answer is " << answer << ", conjugator = ";
		AP.printWord(cout, conjugator);
		cout << endl;
		
		// check the answer.
		if( AP.areEqual(w.conjugateBy(conjugator), u) != yes ) 
			cout << "*** equality test failed! *** " << endl;

		cout << endl << endl;
	}

}

