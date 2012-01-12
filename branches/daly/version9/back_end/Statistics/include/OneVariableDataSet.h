// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of OneVariableDataSet class
//
// Principal Author: Dmitry Bormotov, Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#ifndef _ONEVARIABLEDATASET_H_
#define _ONEVARIABLEDATASET_H_

#include "IStreamPoll.h"
#include "DataSet.h"
#include "DArray.h"

// ------------------------------ OneVariableDataSet ---------------------------------- //

// We don't know yet if SetOf<double> is enough here, or we should use
// class template, i.e. OneVariableDataSet<type>

class OneVariableDataSet
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  OneVariableDataSet( const DataSet& D,int index=0);


  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
    void sort();     ///sort the elements in the data set
    double mean( ) const;  ///get the mean value
    double deviation( ) const;  //get the deviation
    double variance( ) const;  //get the deviation
    double sum() const;    ///get the sum
    double min() const;    ///get the sum
    double max() const;    ///get the sum
    int cardinality() const;   ///get the number of elements
    double standardDeviation() const;  //get the standard deviation
    double range()const;  //get the range=max-min
    double median() const; //get the median
    double firstQuartile() const;  //get the 1/4th element
    double thirdQuartile() const;   ///get the 3/4th element
    double coefficientSkewness() const;  //get the coefficient of skewness
    void printDefinition(ostream& ostr) const;
    int GetNum(double lower,double upper,bool upperIncluded=false) const;
    double operator [] (int index) {return theSet[index];}
    double getLogMean();
    double getLogDeviation();
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  VectorOf<double> theSet;
public:
  DArray<double> theArray;
};

#endif
