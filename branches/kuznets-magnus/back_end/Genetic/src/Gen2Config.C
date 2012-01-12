// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of Gen2Config class
//
// Principal Authors: Dmitry Bormotov, Roman Kuznets
//
// Status: in progress
//
// Revision History:
//
//


#include "Chars.h"
#include "Gen2Config.h"


//------------------------------ Gen2Config -----------------------------------//


Gen2Config::Gen2Config( int pop, int gen, 
			int breed,  int surg, 
			int crossF, int save, 
			bool fit ) 
  : population( pop ),
    generations( gen ),
    breeding( breed ),
    surgery( surg ),
    crossFert( crossF ),
    saving( save ),
    //    gettingRid( getrid ),
    newBlood( 100 - breed - surg - crossF - save ),
    //    elitistSelection( elit ),
    bFitnessScaling( fit )
{ }
  

void Gen2Config::readFrom( istream& istr )
{
  while( !istr.eof() ) {

    char s[500],c;
    istr.getline(s,499,'\n');
    if( strlen(s) == 0 || s[0] == '#' ) continue;

    std::istrstream in(s);
    //
    char var[100];
    int value;
    in.get(var,99,':');
    in.get(c);
    in >> value;

    setVariable(var,value);
  }
}


void Gen2Config::setVariable( const char* varName, int value )
{
  // get rid of all spaces
  char s[100];
  int vnLen = strlen(varName), j = 0;
  for( int i = 0; i < vnLen; ++i )
    if( varName[i] != ' ' )
      s[j++] = varName[i];
  s[j] = 0;

  Chars name(s);
  if( name == Chars("Population") )
    population = value;
  else if( name == Chars("Generations") )
    generations = value;
  else if( name == Chars("Breeding") )
    breeding = value;
  else if( name == Chars("Surgery") )
    surgery = value;
  else if( name == Chars("CrossFert") )
    crossFert = value;
  else if( name == Chars("Saving") )
    saving = value;
  //  else if( name == Chars("GettingRid") )
  //    gettingRid = value;
  else if( name == Chars("NewBlood") )
    newBlood = value;
  //  else if( name == Chars("ElitistSelection") )
  //    elitistSelection = value;
  else if( name == Chars("FitnessScaling") )
    bFitnessScaling = value ? true : false;
  else
    error(Chars("void Gen2Config::setVariable( const char*, int ) : ") +
	  "unknown variable " + varName + ".");
}


void Gen2Config::printOn( ostream& ostr ) const
{
  ostr << "# Population size" << endl
       << "Population : " << population << endl << endl
       << "# Number of generations" << endl
       << "Generations : " << generations << endl << endl
       << "# Percent of breeding" << endl
       << "Breeding : " << breeding << endl << endl
       << "# Percent of surgery " << endl
       << "Surgery : " << surgery << endl << endl
       << "# Percent of cross fertilization " << endl
       << "CrossFert : " << crossFert << endl << endl
       << "# Percent of untouched " << endl
       << "Saving : " << saving << endl << endl
       << "# Have fitness scaling" << endl
       << "FitnessScaling : " << (bFitnessScaling ? 1 : 0) << endl << endl;
}



