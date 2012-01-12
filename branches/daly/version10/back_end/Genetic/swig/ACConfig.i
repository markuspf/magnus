
%module Magnus

%{
#include "ACConfig.h"

%}
class ACConfig
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  //GHNConfig( );
  
  ACConfig( int pop = 50, int gen = MAXINT, int fit_scale = 1, 
	    int cross_prob = 70, int mut_prob = 85, int elit = true, 
	    int penalty = 0, int cross_type = 1,
	    int mut_app_prob = 30, int mut_insert_prob = 20,
	    int mut_delete_prob = 20,
	    int mut_change_prob = 10,
	    int mut_permute_prob = 20,
	    int use_sum_fit = 1,
	    int num_min = 1000,
	    int us_w = 1
	    );
  
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors/Modifiers:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  int populationSize() const;
  
  int numOfGenerations() const ;
  
  bool haveFitnessScaling() const;

  bool useSumFitness() const;

  double chanceOfCrossover( ) const ;
  
  double chanceOfMutation( ) const ;
  
  int numOfElitistSelection( ) const ;
  
  int penaltyRate() const ;
  
  int crossoverType() const ;
  
  double chanceOfMutationAppend( ) const;
  
  double chanceOfMutationInsert( ) const ;
  
  double chanceOfMutationDelete( ) const ;
  
  double chanceOfMutationChange( ) const ;

  double chanceOfMutationPermute() const;

  int numMinmizeAfterOf() const ;
  
  bool useWhitehead() const ;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  // assignment operator supplied by compiler

//  friend ostream& operator << ( ostream& ostr, const ACConfig& C )
    
//  friend istream& operator >> ( istream& istr, ACConfig& C )
   %addmethods{
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}

  }
/*

private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void readFrom( istream& istr );

  void printOn( ostream& ostr ) const;

  void setVariable( const char*, int );
  // used by readFrom() to set variables

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  int population;
  int generations;
  int fitness_scale;
  int crossover_prob; 
  int mutation_prob;
  int elittest; 
  int penalty_rate;
  int crossover_type;
  int mutation_app_prob;
  int mutation_insert_prob;
  int mutation_delete_prob;
  int mutation_change_prob;
  int mutation_permute_prob;
  int use_sum_fitness;
  int num_min_after;
  int use_whitehead;
*/
};
%inline %{
	ACConfig * ACConfigFromString(char* p) {
		strstream s;
		ACConfig* F=new ACConfig();
		s << p << '\0' << flush;
		s >> *F;
		return F;
	}
%}