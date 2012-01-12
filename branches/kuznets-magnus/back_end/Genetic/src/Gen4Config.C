// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of Gen4Config class
//
// Principal Authors: Dmitry Bormotov, Roman Kuznets
//
// Status: in progress
//
// Revision History:
//
//


#include "Chars.h"
#include "Gen4Config.h"


//------------------------------ Gen4Config -----------------------------------//


Gen4Config::Gen4Config( float ds, 
			int breed,  int surg, 
			int crossF, int save, 
			bool fit ) 
  : dihSym( ds ),
    breeding( breed ),
    surgery( surg ),
    crossFert( crossF ),
    saving( save ),
    newBlood( 100 - breed - surg - crossF - save ),
    bFitnessScaling( fit )
{ }
  

void Gen4Config::readFrom( istream& istr )
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


void Gen4Config::setVariable( const char* varName, int value )
{
  // get rid of all spaces
  char s[100];
  int vnLen = strlen(varName), j = 0;
  for( int i = 0; i < vnLen; ++i )
    if( varName[i] != ' ' )
      s[j++] = varName[i];
  s[j] = 0;

  Chars name(s);
  if( name == Chars("DihSym") )
    dihSym = value;
  else if( name == Chars("Breeding") )
    breeding = value;
  else if( name == Chars("Surgery") )
    surgery = value;
  else if( name == Chars("CrossFert") )
    crossFert = value;
  else if( name == Chars("Saving") )
    saving = value;
  else if( name == Chars("NewBlood") )
    newBlood = value;
  else if( name == Chars("FitnessScaling") )
    bFitnessScaling = value ? true : false;
  else
    error(Chars("void Gen4Config::setVariable( const char*, int ) : ") +
	  "unknown variable " + varName + ".");
}


void Gen4Config::printOn( ostream& ostr ) const
{
  ostr << "# Percent of dihedral-symmetric pairs : " << endl
       << "DihSym : " << dihSym << endl << endl
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



