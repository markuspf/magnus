// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal search for a counterexample of Hanna Neumann conjecture
//           using Gen2Subgroups class and genetic approach.
//
// Principal Author: Roman Kuznets
//
// Status:
//
// Revision History:
//


//#include <values.h>
#include <vector>
//#include <algorithm>
//#include <iostream>
//#include<iterator>


//#include "global.h"
#include "Gen2Config.h"
//#include "RandomNumbers.h"
#include "GlobalRoulette.h"
#include "Gen2Subgroups.h"
//#include "GlobalRandom.h"
using namespace std;




void writePairs2File( ofstream& out, const vector<Gen2Subgroups> pop )
{
  out << pop.size() << endl;
  for( int i = 0; i < pop.size(); i ++ )
    {
      out < pop[i].generatorsSG1() < pop[i].generatorsSG2() ;
    }
}




vector<Gen2Subgroups> readPairsFromFile ( ifstream& in )
{
  int popSize;
  in >> popSize;
  cout << popSize << endl;
  vector<Gen2Subgroups> P;
  P.reserve( popSize );
  for( int i = 0; i < popSize; i ++ )
    {
      SetOf<Word> a, b;
      in > a;
      in > b;
      P.push_back( Gen2Subgroups(a,b) );
    }
  return P;
}




int largerFitness(const Gen2Subgroups P1, const Gen2Subgroups P2)
  // this function is used in sorting routine to compare fitnesses of two pairs of groups
{
  return ( P1.fitness() > P2.fitness() );
}




Gen2Subgroups createNewPair( const float ds, const int popSize ) 
  // parameter `ds' is the probability of choosing dihedral/symmetric pair as opposed to a Karass-Solitar one.
  // `popSize' is the size of the population (used in Karass-Solitar pairs)
{
  Gen2Subgroups P;

  // Variable used to control the size of the created dihedral and symmetric groups.
  // Does not affect Karass-Solitar ones.
  // Larger values allow for greater diversity but slow down the algorithm 
  // The value of the parameter used to create the groups is chosen at random between 1 and `dsGroupSize' 
  // Currently is put to coincide with `popSize'.
  // Should be scaled down if the algorithm works too slowly.
  int dsGroupSize = popSize;

  // random choice of dihedral/symmetric or Karass-Solitar
  float op = globalUniformRandom.rand();

  // case of dihedral/symmetric
  if ( op <= ds ) 
    { 
      SetOf<Word> group1 = P.createDihedralSubgroup( dsGroupSize, true );
      //      cout << "Created dihedral group with papameter between 2 and " << dsGroupSize  << endl;
      //      Second parameter is boolean. 
      //      True means choose randomly between 2and dsGroupSize.
      //      False means use dsGroupSize
      SetOf<Word> group2 = P.createSymmetricSubgroup( dsGroupSize, true );
      //      cout << "Created symmetric group with papameter between 2 and " << dsGroupSize  << endl;
      //      Second parameter is boolean. 
      //      True means choose randomly between 2and dsGroupSize.
      //      False means use dsGroupSize
      return Gen2Subgroups( group1, group2 );
    }
  // case of Karass-Solitar pair
  else 
    { 
      int n = globalUniformRandom.rand( 2, popSize );
      P = P.createKarSol2Subgroups( n );
      cout << "Created Karass-Solitar pair with parameter n = " << n << endl;
      return P;
    }
}





//===============================================================================//
//                 Main procedure                                                //
//===============================================================================//
int main ( )
{
  timeval tp;
  gettimeofday( &tp, 0 );

  cout << "You are about to search for a counterexample to Hanna Neumann conjecture" << endl
       << "Choose one of the options:" << endl
       << "(1) Start a new experiment" << endl
       << "(2) Reproduce a saved experiment and continue it with the original seed for random variables" << endl
       << "(0) Quit the program" << endl
       << "What is your choice? ";
  int option;
  string badChars;
  while ( !(cin >> option) || (option > 2) || (option < 0) ) 
    {
      cin.clear();
      cin >> badChars;
      cout << "Please, input an integer between 0 and 3!" << endl;
    }

  // setting the rules for generating initial population and repopulating each generation
  // fraction of dihedral-symmetric groups (the other possibility is Karass-Solitar group 
  // 'dihSymmFrac' stores the chance of creating dihedral-symmetric group
  float dihSymmFrac = .5;

  Gen2Config c;
  int popSize;
  vector<Gen2Subgroups> pop, newPop;
  int numOfGens;
  float crossRate;
  float mutRate;
  float fertRate;
  float saveRate;
  float replaceRate;
  int currGen;
  

  if ( option == 0 ) 
    // quitting the program
    {
      exit(0);
    }

  else if ( option == 1 )
    // initializing a new experiment from file test/in2
    {
      // initializing the global random variable used throught the genetic search.
      cout << "Seed value in this experiment is " << tp.tv_usec << endl;
      globalUniformRandom.reseed( tp.tv_usec );
      cout < globalUniformRandom;;
      cout << endl;
      
      
      /*
      //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // temporary section
      //time evaluation for symmetric groups
      ofstream out( "symmtime" );
      if ( !out )
      {
      cerr << "error opening file symmtime" << endl;
      }
      for( int i = 3; i < 101; i ++ )
      {
      Gen2Subgroups P;
      timeval tBegin, tEnd;
      gettimeofday( &tBegin, 0 );
      SetOf<Word> group1 = P.createDihedralSubgroup( 4, false );
      SetOf<Word> group2 = P.createSymmetricSubgroup( i, false );
      P = Gen2Subgroups( group1, group2 );
      gettimeofday( &tEnd, 0 );
      out << "Time needed for groups of size " << i << " is " << tEnd.tv_sec - tBegin.tv_sec << " seconds." << endl;
      }
      out.close();
      // end of temprorary section
      exit(0);  
      */
      
      
      
      // reading the parameters of the experiment from the file `test/in2' to variable `c'
      // parameters include the size of each generation, the number of generations and other genetic algorithm parameters
      ifstream in( "test/in2" );
      if( ! in ) 
	{
	  cerr << "error opening file test/in2, aborting" << endl;
	  exit( 1 );
	}
      in >> c;
      cout << endl;
      cout << "File test/in2 successfully downloaded" << endl;
      in.close();
      cout << c << endl;
      cout << "===============================================================================" << endl;
      
      
      

      
      
      cout << "Initializing the original population..." << endl
	   << "Enumeration of pairs starts at 0 (C style)." << endl;
      
      
      // initializing vectors for storing the population
      popSize = c.populationSize();
      pop.reserve( popSize );
      newPop.reserve( popSize );
      
      
      //================================================//
      // creating the initial population                //
      //================================================//
      for( int i = 0; i < popSize; ++ i ) 
	{
	  pop.push_back( createNewPair( dihSymmFrac, popSize ) );
	  cout << "Created pair #" << i << endl;
	}
      
      
      cout << "Initialization complete "<< endl;
      cout << "===============================================================================" << endl;
      
      
      // parameters from `test/in2'
      numOfGens = c.numOfGenerations();
      crossRate = c.fractionForBreeding();
      mutRate = c.fractionForSurgery();
      fertRate = c.fractionForCrossFert();
      saveRate = c.fractionForSaving();
      replaceRate = c.fractionForNewBlood();
      
      currGen = 0;
    }
  // END OF OPTION 1
      
  else if ( option == 2 )
    // reading the experiment from a file
    // together with the old seed
    // Continuing the experiment from the place it was interrupted
    {




      // setting the rules for generating initial population and repopulating each generation
      // fraction of dihedral-symmetric groups (the other possibility is Karass-Solitar group 
      // 'dihSymmFrac' stores the chance of creating dihedral-symmetric group
      dihSymmFrac = .5;

      
      cout << "Enter the file with the saved experiment: ";
      char filename[256];
      cin >> filename;
      ifstream in( filename );
      if( ! in ) 
	{
	  cerr << "error opening file " << filename << ", aborting" << endl;
	  exit( 1 );
	}

      // reading the parameters of the experiment from the file `filename' 
      // parameters include the size of each generation, the number of generations and other genetic algorithm parameters

      // reading the original seed
      int tmpSeed;
      in >> tmpSeed;
      in > globalUniformRandom;

      // reading the number of saved generation in the original experiment
      in >> currGen;
      // reading the population itself
      pop = readPairsFromFile ( in );
      // reading the original configuration file
      in >> c;
      cout << c << endl;
      in.close();
      cout << endl;
      cout << "File " << filename << " successfully downloaded" << endl;

      
      cout << "The fitnesses of the population read from the saved experiment are..." << endl;
      popSize = c.populationSize();
      for( int i = 0; i < popSize; i ++ ) 
	{
	  cout << pop[i].fitness() << " ";
	}    
      cout << endl << endl;
      newPop.reserve( popSize );
      int fit[ popSize ];

      // parameters from `filename'
      numOfGens = c.numOfGenerations() - currGen;
      crossRate = c.fractionForBreeding();
      mutRate = c.fractionForSurgery();
      fertRate = c.fractionForCrossFert();
      saveRate = c.fractionForSaving();
      replaceRate = c.fractionForNewBlood();
 

      cout << "This was generation #" << currGen << " in the saved experiment." << endl
	   << "The original experiment was supposed to compute " << numOfGens << " generations more." << endl
	   << "Would you like to change the number of the generations to compute? (y/n) ";
      char yn = ' ';
      cin >> yn;
      while ( (yn != 'y') && (yn != 'n') )
	{
	  cout << "Please enter 'y' or 'n': ";
	  cin >> yn;
	}
      if ( yn == 'y' ) 
	{
	  cout << "Enter the number of new generations you want to compute: ";
	  int addNumOfGens;
	  while ( !(cin >> addNumOfGens) || (addNumOfGens < 0) ) 
	    {
	      cin.clear();
	      cin >> badChars;
	      cout << "Please, input a non-negative integer" << endl;
	    }
	  numOfGens = currGen + addNumOfGens;
	}
      else if ( numOfGens <= 0 )
	{
	  cout << "The saved experiment was completed." << endl
	       << "Would you like to see its results? ";
	  char yn1;
	  cin >> yn1;
	  while ( (yn1 != 'y') && (yn1 != 'n') )
	    {
	      cout << "Please enter 'y' or 'n': ";
	      cin >> yn1;
	    }
	  if ( yn1 == 'y' )
	    {
	      cout << "Generation: " << currGen 
		   << "   Best fitness: " << pop[0].fitness() 
		   << endl
		   << "(ranks of subgroups are " << pop[0].r1 
		   << " and " << pop[0].r2 << ";"
		   << " rank of intersection is " << pop[0].rInt 
		   << ")" << endl;
	      SubgroupGraph A( 2, pop[0].generatorsSG1() );
	      
	      int indA = A.findIndex();
	      VectorOf<Word> nbA = A.nielsenBasis();
	      if ( indA == 1 ) 
		{
		  cout << "The 1st subgroup is the ambient free group :(" << endl;
		}
	      else
		{
		  cout << "Index of the 1st subgroup is " << indA << endl;
	  cout << "Nielsen basis for the 1st subgroup is " << nbA
	       << " of size " << nbA.length() 
	       << endl;
		}
	      
	      SubgroupGraph B( 2, pop[0].generatorsSG2() );
	      int indB = B.findIndex();
	      VectorOf<Word> nbB = B.nielsenBasis();
	      if ( indB == 1 ) 
		{
		  cout << "The 2nd subgroup is the ambient free group :(" << endl;
		}
	      else
		{
		  cout << "Index of the 2nd subgroup is " << indB << endl;
	  cout << "Nielsen basis for the 2nd subgroup is " << nbB 
	       << " of size " << nbB.length() 
	       << endl;
		}
	      
	      if ( A.equalTo( B ) ) 
		{
		  cout << "Subgroups are equal :(" <<  endl;
		}
	      
	    }
	}
      // cout << "Reading a saved experiment complete "<< endl;
      //      cout << "===============================================================================" << endl;
    }

  int fit[ popSize ];

  //================================================//
  // the main loop                                  //
  //================================================//
  for ( int g = currGen; g <= numOfGens; ++ g ) 
    {
      // sorting the population in order of descending fitness
      cout << "Sorting for generation " << g << " ..." << endl;
      std::partial_sort( pop.begin(), pop.begin() + popSize, pop.end(), largerFitness );
      cout << endl << "The fitnesses of generation " << g << " in descending order are:" << endl;

      cout << popSize << endl;
      cout << fit[0] << endl;
      fit[0] = 45;
      cout << fit[0] << endl;

      for( int i = 0; i < popSize; ++ i ) 
	{
	  fit[i] = pop[i].fitness();
	  cout << fit[i] << " ";
	}    
      cout << endl;


      cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
      ofstream out( "GenerationSnapshot" );
      if ( !out )
	{
	  cerr << "error opening file GenerationSnapshot" << endl;
	}
      
      vector<Gen2Subgroups> tmpPop;
      tmpPop.reserve( popSize );
      out << tp.tv_usec << endl;
      out < globalUniformRandom;
      out << endl;
      out << g << endl;
      writePairs2File( out, pop );
      // in any snapshot file, Gen2Config variable must be recorded last 
      // due to the way it is read from the file
      out << c;
      out.close();


      //================================================//
      // printing current results                       //
      //================================================//
      cout << "Generation: " << g 
	   << "   Best fitness: " << fit[0] 
	   << endl
	   << "(ranks of subgroups are " << pop[0].r1 
	   << " and " << pop[0].r2 << ";"
	   << " rank of intersection is " << pop[0].rInt 
	   << ")" << endl;
      SubgroupGraph A( 2, pop[0].generatorsSG1() );

      int indA = A.findIndex();
      VectorOf<Word> nbA = A.nielsenBasis();
      if ( indA == 1 ) 
	{
	  cout << "The 1st subgroup is the ambient free group :(" << endl;
	}
      else
	{
	  cout << "Index of the 1st subgroup is " << indA << endl;
	  cout << "Nielsen basis for the 1st subgroup is " << nbA
	       << " of size " << nbA.length() 
	       << endl;
	}

      SubgroupGraph B( 2, pop[0].generatorsSG2() );
      int indB = B.findIndex();
      VectorOf<Word> nbB = B.nielsenBasis();
      if ( indB == 1 ) 
	{
	  cout << "The 2nd subgroup is the ambient free group :(" << endl;
	}
      else
	{
	  cout << "Index of the 2nd subgroup is " << indB << endl;
	  cout << "Nielsen basis for the 2nd subgroup is " << nbB 
	       << " of size " << nbB.length() 
	       << endl;
	}
      
      if ( A.equalTo( B ) ) 
	{
	  cout << "Subgroups are equal :(" <<  endl;
	}
      
      // nothing needs to be done, if this is the last generation.
      if ( g < numOfGens )
	{
	  // making fitness values positive with minimum value equal to 1
	  int base = abs( fit[popSize - 1] ) + 1;
	  for( int i = 0; i < popSize; ++ i )
	    fit[i] += base;
	  
	  // fitness scaling if specified in `test/in2'
	  if( c.haveFitnessScaling() )
	    for( int i = 0; i < popSize; ++ i )
	      fit[i] = fit[i] * fit[i];
	  
	  
	  // `evolveSize' is the best fitted part of the population 
	  // that participates in  breeding, surgery, part of it is kept unchanged, another part is cross fertilized with the worst.
	  // The size of it is specified in `test/in2'
	  int evolveSize = (int) ( popSize * (1 - replaceRate) );
	  
	  // initializing auxiliary vectors used to queue pairs for breeding and cross fertilization
	  vector<Gen2Subgroups> breedVector, crossFertVector;
	  breedVector.reserve( evolveSize );
	  crossFertVector.reserve( evolveSize );
	  
	  
	  // initializing counters for statistics on the new population
	  int ibreed = 0;
	  int isurg = 0;
	  int ikeep = 0;
	  int icross = 0;
	  
	  //================================================//
	  // creating the new population                    //
	  //================================================//
	  
	  // All the pairs created are temporarely stored in auxiliary vector newPop
	  cout << "Generation " << g 
	       << ". Working on the best " << evolveSize 
	       << " pairs." << endl;
	  
	  for( int i = 0; i < evolveSize; ++ i ) 
	    {
	      // throwing a dice on which operation to apply to pair #i
	      float op = globalUniformRandom.rand();
	      op = (1 - replaceRate) * op;
	      
	      // case of breeding: queue into `breedVector'
	      if( op <= crossRate ) 
		{
		  breedVector.push_back( pop[i] );
		  cout << "Pair #" << i << ": prepare for breeding " << endl;
		  ibreed ++;
		}
	      
	      // case of surgery: perform surgery and store in `newPop'
	      else if( op <= crossRate + mutRate ) 
		{
		  newPop.push_back( pop[i].surgery() );
		  cout << "Pair #" << i << " underwent surgery." << endl;
		  isurg ++;
		}
	      
	      // case of keeping unchanged: simply store in `newPop'
	      else if( op <= crossRate + mutRate + saveRate ) 
		{
		  newPop.push_back( pop[i] );
		  cout << "Pair #" << i << " is transferred to the next generation." << endl;
		  ikeep ++;
		}
	      
	      // case of cross fertilizing: queue into `crossFertVector'
	      else if( op <= crossRate + mutRate + saveRate + fertRate ) 
		{
		  crossFertVector.push_back( pop[i] );
		  cout << "Pair #" << i << ": prepare for cross fertilization." << endl;
		  icross ++;       
		}
	    }
	  
	  // statistics on surgery and pairs unchanged
	  cout << "Generation " << g 
	       << ": " << isurg << " pairs underwent surgery. " 
	       << ikeep << " pairs survived unchanged." 
	       << endl;
	  cout << "Generation " << g + 1 << ": " 
	       << newPop.size() << " new pairs created so far." 
	       << endl;
	  
	  
	  // initializing for the queued breeding and cross fertilization
	  int breedSize = breedVector.size();
	  int crossFertSize = crossFertVector.size();
	  int breedFit[ breedSize ];
	  for (int j = 0; j < breedSize; ++ j ) 
	    {
	      breedFit[j] = breedVector[j].fitness();
	    }
	  
	  // performing breeding of the queued pairs
	  cout << "Generation " << g << ": " 
	       << breedSize << " pairs to breed. Start breeding..." 
	       << endl;
	  RouletteWheel<int> wheel( breedSize, breedFit );
	  for( int j = 0; j < breedSize; ++ j ) 
	    {
	      int i1 = wheel.GetIndex();
	      newPop.push_back( breedVector[j].breed( breedVector[i1] ) );
	      cout << "Breeding pair #" << j 
		   << " with pair #" << i1 
		   << " completed" << endl;
	    }
	  cout << "Generation " << g << ": " << "Breeding completed " << endl;
	  cout << "Generation " << g + 1 << ": " 
	       << newPop.size() << " new pairs created so far." 
	       << endl;
	  
	  // performing cross fertilization of the queued pairs
	  cout << "Generation " << g 
	       << ": " << crossFertSize << " pairs to cross fertilize with the worst pairs. Start cross fertilization..." 
	       << endl;
	  for( int i = 0; i < crossFertSize; ++ i ) 
	    {
	      int i1 = globalUniformRandom.rand( 0, crossFertSize - 1 );
	      newPop.push_back( crossFertVector[i].breed( pop[popSize - i1 - 1] ) );
	      cout << "Cross fertilizing pair #" << i 
		   << " with subgroup pair #" << popSize - i1 - 1 
		   << " completed." << endl;
	    }
	  cout << "Generation " << g << ": " << "Cross fertilization completed " << endl;
	  int createdPairs = newPop.size();
	  cout << "Generation " << g + 1 
	       << ": " << createdPairs << " new pairs created so far." 
	       << endl;
	  cout << "===============================================================================" << endl;
	  
	  
	  //================================================//
	  // repopulating generation g + 1                  //
	  //================================================//
	  cout << "Creating the remaining new pairs for generation " << g + 1 << endl;
	  
	  // cleansing vector `pop'
	  pop.erase( pop.begin(), pop.end() );
	  
	  // copying the results of evolution from `newPop'
	  for( int i = 0; i < createdPairs; ++ i ) 
	    {
	      pop.push_back( newPop[i] );
	    }
	  
	  // cleansing vector `newPop'
	  newPop.erase( newPop.begin(), newPop.end() );
	  
	  
	  // creating the lacking pairs 
	  for( int i = 0; i < popSize - createdPairs; ++ i ) 
	    {
	      pop.push_back( createNewPair( dihSymmFrac, popSize ) );
	      cout << "Generation " << g + 1 << ": " 
		   << "Pair #" << createdPairs + i << " is created." 
		   << endl;
	    }    
	}
    }
}

