// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of TwoVariableDataSet class
//
// Principal Author: Dmitry Bormotov, Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#ifndef _CROSSDATASET_H_
#define _CROSSDATASET_H_
#include "IStreamPoll.h"
#include "DataSet.h"
#include "DArray.h"
#include "OneVariableDataSet.h"

// ------------------------------ CrossDataSet ---------------------------------- //

// We don't know yet if SetOf<double> is enough here, or we should use
// class template, i.e. OneVariableDataSet<type>

class CrossDataSet
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  CrossDataSet( const DataSet& D);

  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


public:
   double GetX1();
   double GetX2();
   double GetY1();
   double GetY2();
   int GetRow();
   int GetColumn();
   double GetXSquare();
private:
   bool CheckIntegrality();
   double GetRi(int i);
   double GetCj(int j);
   double GetT();
   double GetOij(int i,int j);
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
private:
  OneVariableDataSet* m_pDs1;
  OneVariableDataSet* m_pDs2;
  const DataSet& m_Ds;
};

#endif
