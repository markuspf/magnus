// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: An experiment which looks for a counter-example to the
//           Hanna Neumann conjecture: if A and B are finitely generated
//           subgroups of a free group of finite rank, and I is the
//           intersection of A and B, then rk(I)-1 <= (rk(A)-1)(rk(B)-1).
//
// Rewritten by Tim Daly
// Principal Author: Roger Needham
//
// Status: Useable.
//
// Revision History:
//
// Next implementation steps:


#include <iostream.h>

#include "String.h"
#include "SubgroupGraph.h"
#include "FreeGroupRep.h"

VectorOf<Word> randomSet( )
// Quick-n-dirty way to make a random set of words on two group generators.
// The size of the set is uniformly random between 100 and 500.
// The lengths of the words are uniformly random between 10 and 75.
{
//  int size = 1 + abs((int)(rand() % 501));
  int size = 1 + abs((int)(rand() % 5));
  VectorOf<Word> result(size);
  while ( size-- ) {
//	 int len = 10 + abs( (int)(rand() % 66) );
	 int len = 1 + abs( (int)(rand() % 6) );
	 VectorOf<Generator> v(len);
	 int i = 0;
	 while ( i < len ) {
		int gen = rand();
		if ( gen & 128 )
		  gen = 1 + ( (gen & 64) ? 1 : 0 );
		else
		  gen = -1 - ( (gen & 64) ? 1 : 0 );
		if ( i == 0 || v[i - 1] != -gen )
		  v[i++] = gen;
	 }
	 result[size]=Word(v);
//	 result |= w.freelyReduce();
//	 result |= Word(v); // No need to freely reduce.
  }
  return result;
}

String CharstoString(VectorOf<char> longwrd)
{ String result = "";
  for(int i=0; i<longwrd.length(); i++)
       result += longwrd[i];
  return(result);
}

String CharstoString2(VectorOf<char> longwrd)
{ String result="";
  char last = longwrd[0];
  int exp = 0;
  for(int i=0; i<longwrd.length(); i++)
  { if (longwrd[i] == last)
    { exp = exp + 1; 
      last=longwrd[i];
    }
    else
    { if (exp > 1)
      { result += last;
        switch(exp)
        { case 2: result +='2'; break;
          case 3: result +='3'; break;
          case 4: result +='4'; break;
          case 5: result +='5'; break;
          case 6: result +='6'; break;
          case 7: result +='7'; break;
          case 8: result +='8'; break;
          case 9: result +='9'; break;
          default: result +='*'; break;
        }
        exp = 1;
      }
      else result += last;
      last = longwrd[i];
    }
  }
  if (exp > 1)
  { result += last;
    switch(exp)
    { case 2: result +='2'; break;
      case 3: result +='3'; break;
      case 4: result +='4'; break;
      case 5: result +='5'; break;
      case 6: result +='6'; break;
      case 7: result +='7'; break;
      case 8: result +='8'; break;
      case 9: result +='9'; break;
      default: result +='*'; break;
    }
  }
  else result += last;
  return(result);
}

VectorOf<VectorOf<char> > wordGen(VectorOf<VectorOf<char> > allWords)
{ char a = 'a';
  char b = 'b';
  char A = 'A';
  char B = 'B';
  int i;
  int len = allWords.length();
  for(i=0; i<len; i++)
    allWords.append(allWords[i]);
  for(i=0; i<len; i++)
    allWords.append(allWords[i]);
  for(i=0; i<len; i++)
    allWords.append(allWords[i]);
  for(i=0; i<len; i++)
    allWords[i].append(a);
  for(i=len; i<len+len; i++)
    allWords[i].append(b);
  for(i=len+len; i<len+len+len; i++)
    allWords[i].append(A);
  for(i=len+len+len; i<len+len+len+len; i++)
    allWords[i].append(B);
  return(allWords);
}
  
  

VectorOf<char> WordtoChars(Word& w)
{ int len = w.length();
  VectorOf<char> result = VectorOf<char>(len);
  for (int i=0; i < len; i++)
  { int v = shortLexIndex(w[i]);
    switch(v)
    { case 0: result[i]='a'; break;
      case 1: result[i]='A'; break;
      case 2: result[i]='b'; break;
      case 3: result[i]='B'; break;
      default: result[i]='*'; break;
    };
  }
  return(result);
}

void printVectorOf(String title, VectorOf<Word> vec)
{ cout << title << "={ ";
  if (vec.length() > 0)
  { cout << CharstoString2(WordtoChars(vec[0]));
    for(int i = 1; i<vec.length(); i++)
      cout << ", " << CharstoString2(WordtoChars(vec[i]));
  }
  cout << " }" << endl;
}

int main(int a, char **c )
{
  cout << "\nThis experiment looks for a counter-example to the\n"
	 << "Hanna Neumann conjecture: if A and B are finitely generated\n"
	 << "subgroups of a free group of finite rank, and I is the\n"
	 << "intersection of A and B, then rk(I)-1 <= (rk(A)-1)(rk(B)-1).\n\n"
	 << "The generators of A and B are chosen randomly, the number being\n"
	 << "uniformly random between 100 and 500. The word lengths of the\n"
	 << "generators are uniformly random between 10 and 75.\n"
	 << "The ambient free group has rank 2.\n" << endl;


  cout << "\nHow's about a random seed: ";
  long seed;
  cin >> seed;
  cout << "seed = " << seed << endl;
  srand(seed);

  long count = 0;

  while ( 1 ) {

	 VectorOf<Word> AGens = randomSet();
	 VectorOf<Word> BGens = randomSet();
 
	 SubgroupGraph SGGA(2, AGens);
	 SubgroupGraph SGGB(2, BGens);
	 SubgroupGraph SGGI = SGGA.intersection(SGGB);
         VectorOf<Word> nba = SGGA.nielsenBasis();
         VectorOf<Word> nbb = SGGB.nielsenBasis();
         VectorOf<Word> nbi = SGGI.nielsenBasis();

    #ifdef DEBUG
	 // These will print output if anything is bad:
	 SGGA.consistentData();
	 SGGB.consistentData();
	 SGGI.consistentData();
    #endif
	 int Arank = SGGA.rank();
         int Brank = SGGB.rank();
         int Crank = SGGI.rank();

   VectorOf<VectorOf<char> > allWords = VectorOf<VectorOf<char> >(0);
   VectorOf<char> achar;
   achar.append('a');
   VectorOf<char> Achar;
   Achar.append('A');
   VectorOf<char> bchar;
   bchar.append('b');
   VectorOf<char> Bchar;
   Bchar.append('B');
   allWords.append(achar);
   allWords.append(Achar);
   allWords.append(bchar);
   allWords.append(Bchar);
   for (int i=0; i<allWords.length(); i++)
     cout << i << "=" << CharstoString(allWords[i]) << endl;
   allWords = wordGen(allWords);
   for (int i=0; i<allWords.length(); i++)
     cout << i << "=" << CharstoString(allWords[i]) << endl;
   allWords = wordGen(allWords);
   for (int i=0; i<allWords.length(); i++)
     cout << i << "=" << CharstoString(allWords[i]) << endl;
   exit(0);
   if (Arank > 15) // turn off output
{     if (Arank == 1)
        printVectorOf("neilsenBasis(A)",nba);
     cout << "------------------------------------------------------" << endl;
     if (Crank > 0)
     { if ( (Crank > 0) && (Crank != Arank) && (Crank != Brank))
         cout << "  INTERESTING CASE" << endl;
       if ( (Crank - 1) == (Arank - 1) * (Brank - 1) ) 
         cout << "  BOUNDARY CASE OF EQUALITY" << endl;

       count = count + 1;
       cout << "count=" << count 
            << " A=" << Arank 
            << " B=" << Brank 
            << " C=" << Crank << endl;
       printVectorOf("neilsenBasis(A)",nba);
       printVectorOf("neilsenBasis(B)",nbb);
       printVectorOf("neilsenBasis(C)",nbi);
       printVectorOf("A",AGens);
       printVectorOf("B",BGens);

       if ( (Crank - 1) > (Arank - 1) * (Brank - 1) ) 
       {  cout << "\n\n**** HAVE A COUNTER-EXAMPLE! ****\n\n";
          exit(0);
       }
    }
  } // turn off output

  }
}
