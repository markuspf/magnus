// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of DataSet class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#ifndef _DataSet_H_
#define _DataSet_H_
#include "IStreamPoll.h"
#include "Vector.h"

// ------------------------------ DataSet ---------------------------------- //

// We don't know yet if SetOf<double> is enough here, or we should use
// class template, i.e. DataSet<type>

class DataSet
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  DataSet( ) { }

  DataSet( VectorOf<double> set )  { 
    theSets.append(set); }
 
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


  VectorOf<double> getSet(int i) const { return theSets[i]; }
  int getDimension( ) const { 
     return theSets.length(); 
  }
 
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IO:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  friend ostream& operator << ( ostream& ostr, const DataSet& set )
  {
    for(int i=0;i<set.getDimension();i++)
        ostr << "{" << set.theSets[i] << "}";
    ostr << "}" <<endl;
  }
  
  friend IStreamPoll operator >> ( istream& istr, DataSet& set )
  {
    Chars errMsg;
    set.readFrom(istr, errMsg);
    return IStreamPoll(istr, errMsg);
  }


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void eatWhite( istream& istr, int& pos );
  
  void readFrom( istream& istr, Chars& errMsg );

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  VectorOf< VectorOf<double> > theSets;
};

#endif
