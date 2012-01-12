///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "Config.h"
%}


class GHNConfig
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  //GHNConfig( );
  
  GHNConfig( int pop = 50, int gen = -1, int cross = 100, 
	     int mut = 100, int elit = 1, bool fit = true );
  
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int populationSize( ) const ;

  int numOfGenerations( ) const ;

  double chanceOfCrossover( ) const ;
  // returns value betweeen 0 and 1
  
  double chanceOfMutation( ) const ;
  // returns value betweeen 0 and 1
  
  bool haveElitistSelection( ) const ;

  bool haveStrongElitistSelection( ) const ;

  bool haveFitnessScaling( ) const ;
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  // assignment operator supplied by compiler

///  friend ostream& operator << ( ostream& ostr, const GHNConfig& C )
    
///  friend istream& operator >> ( istream& istr, GHNConfig& C )
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
  int crossover;
  int mutation;
  int elitistSelection;
  bool bFitnessScaling;
*/
};
%inline %{
	GHNConfig * GHNConfigFromString(char* p) {
		strstream s;
		GHNConfig* F=new GHNConfig();
		s << p << '\0' << flush;
		s >> *F;
		return F;
	}
%}