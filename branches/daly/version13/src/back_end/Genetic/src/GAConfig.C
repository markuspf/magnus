// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of HNConfig class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "GAConfig.h"


// ----------------------------- GAConfig ---------------------------------- //


GAConfig::GAConfig( int popSize, int numOfGens, int crossChance, 
		    int mutChance, ElitistSelectionType elitType, 
		    bool fitScaling, int Report, int FitnessPrint,
		    int PopulationPrint )
  : population( popSize ),
    generations( numOfGens ),
    crossover( crossChance ),
    mutation( mutChance ),
    elitistSelection( elitType ),
    bFitnessScaling( fitScaling ),
    theReport( Report ),
    theFitnessPrint( FitnessPrint ),
    thePopulationPrint( PopulationPrint )
{ }
  

void GAConfig::readFrom( istream& istr )
{
  while( !istr.eof() ) {

    char s[500],c;
    istr.get(s,499,'\n');
    istr.get(c);
    if( strlen(s) == 0 || s[0] == '#' ) continue;

    istrstream in(s);
    char var[100], value[500];
    in.get(var,99,':');
    in.get(c);

    in.get(value,499,'\n');
    in.get(c);

    eatWhite(var);
    eatWhite(value);

    setVariable(Chars(var),Chars(value));
  }
}

void GAConfig::eatWhite( char* input )
{
  // get rid of all spaces
  int iLen = strlen(input), j = 0;
  for( int i = 0; i < iLen; ++i )
    if( input[i] != ' ' )
      input[j++] = input[i];
  input[j] = 0;
}

void GAConfig::setVariable( Chars varName, Chars value )
{
  istrstream in(value);
  int iValue;

  if( varName == Chars("Population") ) {
    in >> iValue;
    population = iValue;
  }

  else if( varName == Chars("Generations") ) {
    in >> iValue;
    generations = iValue;
  }

  else if( varName == Chars("Crossover") ) {
    in >> iValue;
    crossover = iValue;
  }

  else if( varName == Chars("Mutation") ) {
    in >> iValue;
    mutation = iValue;
  }

  else if( varName == Chars("ElitistSelection") )
    if ( value == Chars("SIMPLE") )
      elitistSelection = GAConfig::SIMPLE;
    else if ( value == Chars("STRONG") )
      elitistSelection = GAConfig::STRONG;
    else if ( value == Chars("NONE") )
      elitistSelection = GAConfig::NONE;
    else
      error("void GAConfig::setVariable( ... ) : "
	    "Illegal value for ElitistSelection");

  else if( varName == Chars("FitnessScaling") )
    if ( value == Chars("YES") )
      bFitnessScaling = 1;
    else if ( value == Chars("NO") )
      bFitnessScaling = 0;
    else
      error("void GAConfig::setVariable( ... ) : "
	    "Illegal value for FitnessScaling");
  
  else if( varName == Chars("Report") ) {
    in >> iValue;
    theReport = iValue;
  }

  else if( varName == Chars("FitnessPrint") ) {
    in >> iValue;
    theFitnessPrint = iValue;
  }

  else if( varName == Chars("PopulationPrint") ) {
    in >> iValue;
    thePopulationPrint = iValue;
  }


  // put new varName's here


  else
    if( !setUserVariable( varName, value ) )
      error(Chars("void GAConfig::setVariable( ... ) : ") +
	    "unknown variable" + varName + ".");
}

void GAConfig::printOn( ostream& ostr ) const
{
  ostr << "# Population size" << endl
       << "Population : " << population << endl << endl
       << "# Number of generations" << endl
       << "Generations : ";
  if( generations < 0 )
    ostr << "unlimited";
  else
    ostr << generations;
  ostr << endl << endl
       << "# Chance of crossover" << endl
       << "Crossover : " << crossover << endl << endl
       << "# Chance of mutation " << endl
       << "Mutation : " << mutation << endl << endl
       << "# Type of elitist selection" << endl
       << "ElitistSelection : ";
  
  if( elitistSelection == GAConfig::SIMPLE )
    cout << "SIMPLE";
  else  if( elitistSelection == GAConfig::STRONG )
    cout << "STRONG";
  else 
    cout << "NONE";

  cout << endl << endl
       << "# Have fitness scaling" << endl
       << "FitnessScaling : " << (bFitnessScaling ? "YES" : "NO") 
       << endl << endl;

  cout << "# Report every ... generations" << endl
       << "Report : " << theReport << endl << endl
       << "# Print all fitness values every ... generations" << endl
       <<  "FitnessPrint : " << theFitnessPrint << endl << endl
       << "# Print population every ... generations" << endl
       << "PopulationPrint : " << thePopulationPrint << endl << endl;
}



