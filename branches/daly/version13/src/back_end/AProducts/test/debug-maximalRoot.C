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
	// within loop:
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

		// print its decomposition
		VectorOf<Word> deco = AP.decompose(w);
		cout << "Decomposition is: ";
		AP.printVectorOfWords(cout, deco);
		cout << endl;

		// print its reduced decomposition
		VectorOf<Word> rdeco = AP.reducedDecomposition(w);
		cout << "Reduced decomposition is: ";
		AP.printVectorOfWords(cout, rdeco);
		cout << endl;

		// print its normal decomposition
		VectorOf<Word> ndeco = AP.normalDecomposition(w);
		cout << "Normal decomposition is: ";
		AP.printVectorOfWords(cout, ndeco);
		cout << endl;

		// print its cyclic normal decomposition
		VectorOf<Word> cdeco;
		Word conj;
		AP.cyclicDecomposition(w, cdeco, conj);
		cout << "Cyclic normal decomposition is: ";
		AP.printVectorOfWords(cout, cdeco);
		cout << endl;
		
		{ // check APGroup::maximalRoot(...)
			Word root;
			int power;
			AP.maximalRoot(w, root, power);
			cout << "Maximal root is ";

#ifdef DEBUG_APORelator
			cerr << "( " << root << " ) ";
#endif

			AP.printWord(cout, root);
			cout << ", power is " << power << endl;
			Word ww = root;
			for(int i = 1; i < power; ++i)
				ww = ww * root;
			Word nww = AP.normalFormOf(ww);
			Word nw  = AP.normalFormOf(w);
			if( nw != nww ) {
				cout << "  * error: normal form of w is ";
				AP.printWord(cout, nw);
				cout << "\n        normal form of root^power is ";
				AP.printWord(cout, nww);
				cout << endl;
			}
		}

		// check:
		// equality problem for reduced and word
		cout << "Equality test (w, rdeco) ";
		if( AP.areEqual(w, AP.compose(rdeco)) == YES )
			 cout << "passed" << endl;
		else cout << "failed" << endl;

		// equality problem for normal and word
		cout << "Equality test (w, ndeco) ";
		if( AP.areEqual(w, AP.compose(ndeco)) == YES )
			 cout << "passed" << endl;
		else cout << "failed" << endl;

		// equality problem for normal and reduced
		cout << "Equality test (ndeco, rdeco) ";
		if( AP.areEqual(AP.compose(ndeco), AP.compose(rdeco)) == YES )
			 cout << "passed" << endl;
		else cout << "failed" << endl;

		cout << endl << endl;
	}

}
