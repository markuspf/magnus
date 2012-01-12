#include "APwithOneRelator.h"
#include "NormalRandomWord.h"
#include "Automorphism.h"

void testRandom()  
{
	cout << "This test generates random amalgamated products of free groups"
		      " with one relator.\n";

	int seed;
	cout << "Enter an initial seed: ";
	cin >> seed;

	UniformRandom ur(seed);

	VectorOf<Chars> f1_names(ur.rand(1,5));
	int genNameBase = 'a';
	for(int i = 0; i < f1_names.length(); ++i)
		f1_names[i] = Chars( genNameBase+i );

	VectorOf<Chars> f2_names(ur.rand(1,5));
	genNameBase = 'a' + f1_names.length();
	for(i = 0; i < f2_names.length(); ++i)
		f2_names[i] = Chars( genNameBase+i );
	
	FreeGroup F1( f1_names );
	FreeGroup F2( f2_names );

	NormalRandomWord nrw1( 
												F1.numberOfGenerators(),
												5 /* meanLength */, 
												3 /* lengthStddev */,
												seed /* lengthSeed */,
												seed+100 /* generatorSeed */
												);
	NormalRandomWord nrw2( 
												F2.numberOfGenerators(),
												5 /* meanLength */, 
												3 /* lengthStddev */,
												seed+200 /* lengthSeed */,
												seed+300 /* generatorSeed */
												);

	VectorOf<Word> F1_sub(1);
	VectorOf<Word> F2_sub(1);
	F1_sub[0] = nrw1.word().freelyReduce();
	F2_sub[0] = nrw2.word().freelyReduce();

	APwithOneRelator AP(F1, F2, F1_sub, F2_sub);

	cout << "Random generated APORelator group is " << AP << endl << endl;


	int meanLength, lengthStddev, lengthSeed, generatorSeed;

	cout << "Now enter parameters for random word to testing:" << endl;
	cout << "mean length of word = "; cin >> meanLength;
	cout << "word length stddev  = "; cin >> lengthStddev;
	cout << "word length seed    = "; cin >> lengthSeed;
	cout << "generator seed      = "; cin >> generatorSeed;

	cout << endl << "You entered: meanLength=" << meanLength 
		<< ", lengthStddev=" << lengthStddev << ", lengthSeed=" << lengthSeed 
		<< ", genSeed=" << generatorSeed << endl << endl;

	NormalRandomWord randWord(AP.numberOfGenerators(), meanLength, lengthStddev,
														lengthSeed, generatorSeed);

	int maxCentrTests = 10/*1000*/;
	cout << "Finding a centralizer of random generated words " << endl;
	int passed = 0;
	int failed = 0;
	for(i=0; i<maxCentrTests; i++)  {
		Word w = randWord.word();
		
		VectorOf<Word> centralizer = AP.centralizerOf(w);
		
		for(int j = 0; j < centralizer.length(); ++j) {
			if( !AP.commute(w, (Word)centralizer[j]) ) {
				cout << "-- test failed with word ";
				AP.printWord(cout, w);
				cout << endl;
				failed++;
				break;
			}
		}
	}
	passed = maxCentrTests - failed;
	cout << "centralizer test's final result:" << endl;
	cout << " + " << passed << " tests passed.\n";
	if( failed ) cout << " - " << failed << " tests failed.\n";
}


void testManually() 
{
	APwithOneRelator AP;

	cout << "Enter an amalgamated product of free group with one relator "
		      "(e.g < <a,b>*<c,d>; a = c>): "
			 << endl;
	Chars errMesg = cin >> AP; 
	if( errMesg.length() > 0 ) {
		cout << errMesg << endl;
		return;
	}
	cout << "You entered: " << AP << endl << endl;

	cout << "Testing the amalgamated product of free groups: " << endl;

	while(1)  {
		cout << "Enter your word (or 1 to quit): " << endl;
		Word w = AP.readWord(cin, errMesg);
		if( errMesg.length() > 0 )  {
			cout << endl << errMesg << endl;
			return;
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





void main()
{
//	testManually();
	testRandom();
}


