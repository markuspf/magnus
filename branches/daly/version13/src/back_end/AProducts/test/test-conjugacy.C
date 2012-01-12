#include "APwithOneRelator.h"
#include "NormalRandomWord.h"

void main()  
{
	cout << "This program tests random generated amalgamated products of free groups"
		      " with one relator and pairs of random generated words to be conjugated.\n";

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

	{
		int maxConjTests = 10000;
		cout << "Pass 1: test 10000 pairs of always conjugated words." << endl;
		int passed = 0;
		int failed = 0;
		Word w, u, trueConj, foundConj;
		for(i=0; i<maxConjTests; i++)  {
			w = randWord.word();
			trueConj = randWord.word();
			u = w.conjugateBy(trueConj);
			u = u.freelyReduce();

			if( AP.conjugacyProblem(w, u, foundConj) != yes ||
				 AP.areEqual(w.conjugateBy(foundConj), u) != yes ) {

				cout << "=== Test N " << i << endl;
				cout << "*** conjugacy test failed: foundConj = ";
				AP.printWord(cout, foundConj);
				cout << "; w = ";	
				AP.printWord(cout, w);
				cout << ", trueConj = ";
				AP.printWord(cout, trueConj);
				cout << ", u(w^trueConj) = ";
				AP.printWord(cout, u);
				cout << endl;
			}
		}
		passed = maxConjTests - failed;
		cout << endl << "pass 1 summary results:" << endl;
		cout << " + " << passed << " tests passed.\n";
		if( failed ) cout << " - " << failed << " tests failed.\n";
	}

	{
		int maxConjTests = 10000;
		cout << "Pass 2: test 10000 pairs of random generated words." << endl;
		int passed = 0;
		int failed = 0;
		Word w, u, conjugator;
		for(i=0; i<maxConjTests; i++)  {
			w = randWord.word();
			u = randWord.word();
			if( AP.conjugacyProblem(w, u, conjugator) == yes ) {
				if( AP.areEqual(w.conjugateBy(conjugator), u) != yes ) {
					cout << "Test NN " << i << endl;
					cout << "*** equality test failed: w = ";
					AP.printWord(cout, w);
					cout << ", u = ";
					AP.printWord(cout, u);
					cout << ", conj = ";
					AP.printWord(cout, conjugator);
					cout << endl;
				}
			}
		}
		passed = maxConjTests - failed;
		cout << "pass 2 summary results:" << endl;
		cout << " + " << passed << " tests passed.\n";
		if( failed ) cout << " - " << failed << " tests failed.\n";
	}
}

