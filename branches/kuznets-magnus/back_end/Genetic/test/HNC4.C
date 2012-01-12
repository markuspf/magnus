// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal search for a counterexample of Hanna Neumann conjecture
//           using Gen2Subgroups class and genetic approach.
//           This version works with options from the command line  
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
#include "Gen4Config.h"
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

void printInfoAboutPair ( const Gen2Subgroups pair )
{
  cout << "Fitness is " << pair.fitness() << endl
       << "(ranks of subgroups are " << pair.rank1()
       << " and " << pair.rank2() << ";"
       << " rank of intersection is " << pair.rankInt() 
       << ")" << endl;
  SubgroupGraph A( 2, pair.generatorsSG1() );

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
  
  SubgroupGraph B( 2, pair.generatorsSG2() );
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



int largerFitness(const Gen2Subgroups P1, const Gen2Subgroups P2)
  // this function is used in sorting routine to compare fitnesses of two pairs of groups
{
  return ( P1.fitness() > P2.fitness() );
}




Gen2Subgroups createNewPair( const float ds, const int popSize ) 
  // parameter `ds' is the probability of choosing dihedral/symmetric pair as opposed to a Karass-Solitar one.
  // `popSize' is the size of the population (used in Karass-Solitar pairs)
{
  // Variables used to control the size of the created dihedral and symmetric groups.
  // Does not affect Karass-Solitar ones.
  // Larger values allow for greater diversity but slow down the algorithm 
  // The value of the parameter used to create the groups is chosen at random between 1 and `dsGroupSize' 
  // Currently is put to coincide with `popSize'.
  // Should be scaled down if the algorithm works too slowly.
  int dGroupSize = min( popSize, 100 );
  int sGroupSize = min( popSize, 20 );

  // random choice of dihedral/symmetric or Karass-Solitar
  float op = globalUniformRandom.rand();
  cout << "Operation " << op << endl;

  // case of dihedral/symmetric
  if ( op <= ds ) 
    { 
      SetOf<Word> group1 = Gen2Subgroups::createDihedralSubgroup( dGroupSize, true );
      //      cout << "Created dihedral group with papameter between 2 and " << dsGroupSize  << endl;
      //      Second parameter is boolean. 
      //      True means choose randomly between 2and dsGroupSize.
      //      False means use dsGroupSize
      SetOf<Word> group2 = Gen2Subgroups::createSymmetricSubgroup( sGroupSize, true );
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
      Gen2Subgroups P = Gen2Subgroups::createKarSol2Subgroups( n );
      cout << "Created Karass-Solitar pair with parameter n = " << n << endl;
      return P;
    }
}

//===============================================================================//
//                 Main procedure                                                //
//===============================================================================//
int main ( int argc, char *argv[] )
// argc is the number of the arguments + 1
// arguments are stored as an array of strings argv
{
  // getting a timestamp for the beginning of the program
  timeval tp;
  gettimeofday( &tp, 0 );

  // processing arguments from the command line
  int i = 1;
  string optionName;
  bool optionNew = FALSE;
  bool optionSaved = FALSE;
  bool optionConfigfile = FALSE;
  bool optionFile = FALSE;
  bool optionGen = FALSE;
  bool optionPop = FALSE;
  bool optionHelp = FALSE;
  bool optionResult = FALSE;
  string savedExperimentFile = "test/GenerationSnapshot";
  string configFile = "test/G4ConfigFile";
  string resultsFile = "test/GenerationSnapshot";
  int numOfGens = -1;
  int popSize = -1;

  if ( argc == 1 )
    {
      exit( 0 );
    } // end if help message
  
  //===============================================================//
  // Reading the arguments from command line                       //
  //===============================================================//
  while ( i < argc )
    {
      optionName = argv[i];
      if ( ( optionName == "-new" ) || ( optionName == "-n" ) )
	{
	  optionNew = TRUE;
	  i ++;
	} // end if new
      else if ( ( optionName == "-saved" ) || ( optionName == "-s" ) ) 
	{	
	  optionSaved = TRUE;
	  if ( optionNew )
	    {
	      cout << "Options -new and -saved are incompatible. Please choose one of them." << endl;
	      exit( 1 );
	    } // end if
	  if ( ( i == argc-1 ) || ( argv[i+1][0] == '-' ) )
	    {
	      cerr << "Please enter the name of the file with the saved experiment after option -saved" << endl;
	      exit( 1 );
	    } // end if
	  savedExperimentFile = argv[i+1];
	  i += 2;
	} // end if saved
      else if ( ( optionName == "-configfile" ) || ( optionName == "-c" ) ) 
	{	
	  optionConfigfile = TRUE;
	  if ( ( i == argc-1 ) || ( argv[i+1][0] == '-' ) )
	    {
	      cerr << "Please enter the name of the configuration file as an argument. Aborting" << endl;
	      exit( 1 );
	    } // end if
	  configFile = argv[i+1];
	  i += 2;
	  if ( optionSaved )
	    {
	      cout << "Option -saved does not use a configuration file. Option -configfile ignored." << endl;
	    } // end if
	} // end if configfile
      else if ( ( optionName == "-file" ) || ( optionName == "-f" ) ) 
	{	
	  optionFile = TRUE;
	  if ( ( i == argc-1 ) || ( argv[i+1][0] == '-' ) )
	    {
	      cerr << "Please enter the name of the file to save the experiment after option -file" << endl;
	      exit( 1 );
	    } // end if
	  resultsFile = argv[i+1];
	  i += 2;
	} // end if ofile
      else if ( ( optionName == "-gen" ) || ( optionName == "-g" ) ) 
	{	
	  optionGen = TRUE;
	  if ( i == argc-1 )
	    {
	      cerr << "ERROR: Option -gen requires a number of generations as an argument. Aborting" << endl;
	      exit( 1 );
	    } // end if
	  numOfGens = atoi( argv[i+1] );
	  i += 2;
	  if ( numOfGens <= 0 ) 
	    {
	      cout << "Number of generations must be a positive integer." << endl;
	      exit( 1 );
	    } // end if
	} // end if gen
      else if ( ( optionName == "-pop" ) || ( optionName == "-p" ) ) 
	{	
	  optionPop = TRUE;
	  if ( i == argc-1 )
	    {
	      cerr << "Please enter the population size after option -pop" << endl;
	      exit( 1 );
	    } // end if
	  popSize = atoi( argv[i+1] );
	  i += 2;
	  if ( popSize <= 0 )
	    {
	      cout << "Population size must be a positive integer." << endl;
	      exit( 1 );
	    } // end if
	  if ( optionSaved )
	    {
	      cout << "Option -saved uses the population size from the saved experiment. Option -pop ignored." << endl;
	    } // end if
	} // end if pop
      else if ( ( optionName == "-result" ) || ( optionName == "-r" ) ) 
	{	
	  optionResult = TRUE;
	  if ( optionNew ) 
	    {
	      cout << "Options -new and -result are incompatible. Please choose only one of them." << endl;
	      exit ( 1 );
	    } // end if
	  if ( optionConfigfile )
	    {
	      cout << "Option -result does not use a configuration file. Option -configfile ignored." << endl;
	    } // end if
	  if ( optionFile )
	    {
	      cout << "Option -result does not produce any output. Option -file ignored." << endl;
	    } // end if
	  if ( optionGen )
	    {
	      cout << "Option -result does not compute anything. Option -gen ignored." << endl;
	    } // end if
	  if ( optionGen )
	    {
	      cout << "Option -result does not compute anything. Option -pop ignored." << endl;
	    } // end if
	  i ++;
	} // end if result
      else if ( ( optionName == "-help" ) || ( optionName == "-h" ) ) 
	{	
	  optionHelp = TRUE;
	  cout << "HNC4 searches for a counterexample to Hanna Neumann conjecture" << endl
	       << "using genetic approach." << endl
	       << "It has three modes: -n, -s <filename>, and -s <filename> -r." << endl
	       << "-n (or -new) starts a new experiment." << endl
	       << "-s <filename> (or -saved <filename>) continues an experiment " << endl
	       << "saved in the file filename." << endl
	       << "-s <filename> -r (or -saved <filename> -result) shows the best fitness achieved " << endl
	       << "in an experiment saved in the file filename." << endl
	       << endl
	       << "I. Option -new requires " << endl
	       << "1) A configuration file with genetic parameters, "<< endl
	       << "   given by option -c (or -configfile)." << endl
	       << "2) Size of population, given by option -p (or -pop)." << endl
	       << "3) Number of generations, given by option -g (or -gen)." << endl
	       << "You can also specify the location where current information will be saved" << endl
	       << "after each generation by option -f (or -file)." << endl
	       << "Example: " << endl
	       << "HNC4 -configfile test/G4ConfigFile -pop 20 -gen 20 -file MyExperiment" << endl
	       << endl
	       << "II. Option -saved <filename> -result requires only filename after -saved." << endl
	       << "Example:" << endl
	       << "HNC4 -saved OldExperiment -result" << endl
	       << endl
	       << "III. Option -saved <filename> apart from filename after -saved requires:" << endl
	       << "1) Number of generations, given by option -g (or -gen)." << endl
	       << "You can also specify the location where current information will be saved" << endl
	       << "after each generation by option -f (or -file)." << endl
	       << "Example: " << endl
	       << "HNC4 -saved OldExperiment -gen 20 -file ExtraTwentyGenerations" << endl;
	  exit( 0 );
	} // end if help
      else
	{	
	  cout << "Unknown option " << optionName << endl;
	  exit( 1 );
	} // end if unknown option
    } // end while over i reading the command line arguments
  
  int savedSeed;
  Gen4Config c;
  vector<Gen2Subgroups> pop, newPop;
  float dihSymmFrac;
  float crossRate;
  float mutRate;
  float fertRate;
  float saveRate;
  float replaceRate;
  int currGen;

  //==================================================================//
  // Processing the arguments from the command line                   //
  //==================================================================//


  // Reading the information from files
  if ( optionSaved ) 
    // experiment is read from saved file. Config file is ignored.
    {
      ifstream in( savedExperimentFile.c_str() );
      if( ! in ) 
	{
	  cerr << "Error opening file " << savedExperimentFile << endl;
	  exit( 1 );
	} // end if
      
	  // reading the parameters of the experiment from the file with name stored in savedExperimentFile 
	  // parameters include the size of each generation, the number of generations and other genetic algorithm parameters
      
	  // reading the original seed
      in >> savedSeed;
      in > globalUniformRandom;
      
      // reading the number of saved generation in the original experiment
      in >> currGen;
      // reading the population itself
      pop = readPairsFromFile ( in );
      popSize = pop.size();
      // reading the original configuration file
      in >> c;
      in.close();
      cout << endl;
    } // end if reading the saved experiment from the file
  else if ( optionNew && optionConfigfile )
    // configuration for a new experiment is read from a config file
    {
      // reading the parameters of the experiment from the file with name stored in configFile to variable `c'
      // parameters include the size of each generation, the number of generations and other genetic algorithm parameters
      ifstream in( configFile.c_str() );
      if( ! in ) 
	{
	  cerr << "Error opening file " << configFile << endl;
	  exit( 1 );
	} // end if
      in >> c;
      in.close();
    } // end if reading the configuration for a new experiment



  if ( optionResult )
    // printing the result of a saved experiment
    {
      if ( !optionSaved )
	{
	  cout << "Option -result requires a file with a saved experiment to be given in option -saved." << endl;
	  exit( 1 );
	} // end if
      else
	{
	  cout << "You are about to see the results of the experiment saved in file " << savedExperimentFile << endl;
	  cout << "The fitnesses of the population read from the saved experiment are..." << endl;
	  for( int i = 0; i < popSize; i ++ ) 
	    {
	      cout << pop[i].fitness() << " ";
	    } // end of for i
	  cout << endl;
	  cout << "The best pair of generation " << currGen << " in the saved experiment:" << endl;
	  printInfoAboutPair ( pop[0] );
	  exit( 0 );
	} // end else
    } // end if optionResult


  if ( optionNew )
    // initializing a new experiment 
    {
      if ( !optionConfigfile )
	{
	  cout << "Option -new requires a configuration file to be given in option -configfile" << endl;
	  exit ( 1 );
	} // end if
      if ( !optionPop )
	{
	  cout << "Option -new requires a population size to be given in option -pop" << endl;
	  exit( 1 );
	} // end if
      if ( !optionGen )
	{
	  cout << "Option -new requires a number of generations to be given in option -gen" << endl;
	  exit( 1 );
	} // end if
      if ( optionFile )
	{
	  cout << "The results of the experiment will be saved in file " << resultsFile << endl;
	} // end if
      else
	{
	  cout << "The results of the experiment by default will be saved file " << resultsFile << endl;
	} // end else

      // initializing the global random variable used throught the genetic search.
      //      cout << "Seed value in this experiment is " << tp.tv_usec << endl;
      globalUniformRandom.reseed( tp.tv_usec );
      
      cout << "You are about to start a new genetic search for a counterexample to Hanna Neumann conjecture." << endl
	   << "Parameters of the experiment are:" << endl << endl
	   << "Population size: " << popSize << endl << endl
	   << "Number of generations: " << numOfGens << endl << endl
	   << c << endl;
      
      cout << "Initializing the original population..." << endl
	   << "Enumeration of pairs starts at 0 (C style)." << endl;
      
      
      // initializing vectors for storing the population
      pop.reserve( popSize );
      newPop.reserve( popSize );

      // parameters from configFile
      dihSymmFrac = c.dihSymmFraction();
      cout << "Fraction of dihedral-symmetric is " << dihSymmFrac << endl;
      crossRate = c.fractionForBreeding();
      mutRate = c.fractionForSurgery();
      fertRate = c.fractionForCrossFert();
      saveRate = c.fractionForSaving();
      replaceRate = c.fractionForNewBlood();
   
      
      //================================================//
      // creating the initial population                //
      //================================================//
      for( int i = 0; i < popSize; ++ i ) 
	{
	  pop.push_back( createNewPair( dihSymmFrac, popSize ) );
	  cout << "Created pair #" << i << endl;
	} // end for i
      
      
      cout << "Initialization complete "<< endl;
      cout << "===============================================================================" << endl;
      
      currGen = 0;
    } // end if optionNew initialization

  if ( optionSaved && !optionResult ) 
    // reading the experiment from a file
    // Continuing the experiment from the place it was interrupted
    {
      if ( !optionGen )
	{
	  cout << "Option -saved requires a number of generations to be given in option -gen" << endl;
	  exit( 1 );
	} // end if
      if ( optionFile )
	{
	  cout << "The results of the experiment will be saved in file " << resultsFile << endl;
	} // end if
      else
	{
	  cout << "The results of the experiment by default will be saved in file " << resultsFile << endl;
	} // end else
     
      cout << "The fitnesses of the population read from the saved experiment are..." << endl;
      for( int i = 0; i < popSize; i ++ ) 
	{
	  cout << pop[i].fitness() << " ";
	} // end of for i
      cout << endl << endl;
      newPop.reserve( popSize );
      int fit[ popSize ];

      cout << "The saved experiment was saved at generation #" << currGen <<"." << endl;

      cout << "Computing " << numOfGens << " more generations." << endl;

      // parameters from the saved experiment file
      dihSymmFrac = c.dihSymmFraction();
      crossRate = c.fractionForBreeding();
      mutRate = c.fractionForSurgery();
      fertRate = c.fractionForCrossFert();
      saveRate = c.fractionForSaving();
      replaceRate = c.fractionForNewBlood();
 

      // cout << "Reading a saved experiment complete "<< endl;
      //      cout << "===============================================================================" << endl;
    }

  int fit[ popSize ];

  //================================================//
  // the main loop                                  //
  //================================================//
  for ( int g = currGen; g <= currGen + numOfGens; ++ g ) 
    {
      // sorting the population in order of descending fitness
      // not required on the first run of the cycle from the saved experiment
      if ( ( g != currGen ) || optionNew )
	{
	  cout << "Sorting for generation " << g << " ..." << endl;
	  std::partial_sort( pop.begin(), pop.begin() + popSize, pop.end(), largerFitness );
	  cout << endl << "The fitnesses of generation " << g << " in descending order are:" << endl;

	  for( int i = 0; i < popSize; ++ i ) 
	    {
	      cout << pop[i].fitness() << " ";
	    } // end for i
	  cout << endl;
	} // end if sorting
      

      for( int i = 0; i < popSize; ++ i ) 
	{
	  fit[i] = pop[i].fitness();
	} // end for i
      
      
      cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
      ofstream out( resultsFile.c_str() );
      if ( !out )
	{
	  cerr << "error opening file " << resultsFile << endl;
	  exit ( 1 );
	}
      
      vector<Gen2Subgroups> tmpPop;
      tmpPop.reserve( popSize );
      if ( optionNew )
	{
	  out << tp.tv_usec << endl;
	}
      else
	{
	  out << savedSeed << endl;
	}
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
      cout << "Generation: " << g << ".    " 
	   << "For best pair " << endl;
      printInfoAboutPair( pop[0] );

      
      // nothing needs to be done, if this is the last generation.
      if ( g < numOfGens + currGen)
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
	} // end for g (main loop)
    }
}

