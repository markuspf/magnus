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


#ifndef _TWOVARIABLEDATASET_H_
#define _TWOVARIABLEDATASET_H_
#include "OneVariableDataSet.h"
#include "IStreamPoll.h"
#include "DataSet.h"
#include "DArray.h"

// ------------------------------ OneVariableDataSet ---------------------------------- //

// We don't know yet if SetOf<double> is enough here, or we should use
// class template, i.e. OneVariableDataSet<type>

class TwoVariableDataSet
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  TwoVariableDataSet( const DataSet& D);

  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


public:
  bool IsEqualSize();
  double GetSlope();    ///        =Sxy/Sxx
  double GetIntercept();  ///        =Y-bX
  double GetTValue();    ///          =Slope*sqrt(Sxx*(n-2)/SSE);
  double GetR();         ///          =sqrt((Syy-SSE)/Syy)
  double GetUx();        ///          =Rx-m(m+1)/2
  double GetUy();        ///          =Ry-n(n+1)/2
  double GetSb();        ///     =Se/sqrt(Sxx)
  double GetSa();        ///      =Se*sqrt(1/n+X^2/Sxx)
  int GetSize();
  double GetSUy(double X0);
  double GetSpred(double X0);
  double GetLogA();
  double GetLogB();
  double GetLogLogA();
  double GetLogLogB();
  double getQSqure(double A,double B);
  double getPSqure(double A,double B);
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

private:
  double GetSxy();
  double GetRx();
  double GetSe();
  double GetSxLogy();
  double GetSLogxLogy();
  double GetSLogxLogx();
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
private:
  OneVariableDataSet* m_pDs1;
  OneVariableDataSet* m_pDs2;

};

#endif
