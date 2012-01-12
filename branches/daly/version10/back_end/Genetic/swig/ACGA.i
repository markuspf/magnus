%module Magnus

%{
#include "ACGA.h"

%}


class ACGA
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  ACGA( const FPGroup& G, const ACConfig& config );
  
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  //Trichotomy isTrivial( const Word& u, ostream& out );
  // returns yes if the genetic algorithm could prove that u
  // is trivial; returns dontknow after computing all generations

  bool transform( ostream& out, ostream& best_out );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
//  int fitness(  Word& u, ListOf<Word>& conj );

  Word randomWord( );

///  void printOn( Word& u, const ListOf<Word>& conj, ostream& out ) ;
/*xxwundo  %addmethods{
      	char* print(Word& u, const ListOf<Word>& conj){
		strstream *s=new strstream();
		self->print(u,conj,*s);
		(*s) << '\0' << flush;
		return s->str();
	}
        int Fitness(  Word& u, VectorOf<Word>& conj ){
           int length=conj.length();
           ListOf<Word> list(length);
           for(int i=0;i<length;i++) {
              list[i]=conj[i];
	   }
           return self->fitness(u,list);
        }
   }
*/
/*
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Word mutate( const Word& u );

  int randomGen( );
  
  Word crossover1( const Word& w1,const Word& w2 );
  
  Word crossover2( const Word& w1,const Word& w2 );
 
  int genToInsertAfter( Generator g );

  int genToInsertBefore( Generator g );

  int genToInsert( Generator g1, Generator g2 );

  Word randomPermutation();

  Word randomWordInGroup();
    
  void applyWhitehead( ostream& out);
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  FPGroup theGroup;
  ACConfig cfg;

  //friend main();
 
//  SetIterator<Word> I;
  UniformRandom r;
  int numberOfRelators;
  int numberOfGens;
  int numberOfTrans;
  VectorOf<Word> transformations;
  VectorOf<Word> relatorsVector;
  VectorOf<Word> transferImage;
  VectorOf<Word> currentBest;
  VectorOf<Chars> trNames;
  AssociationsOf<int,int> reduces;

  int g;
  Word finalResult;

  int numberOfGenTransf;
  int numberOfRelTransf;
  bool useWhitehead;
*/
};


