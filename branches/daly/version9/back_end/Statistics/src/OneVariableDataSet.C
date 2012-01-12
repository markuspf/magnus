// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of OneVariableDataSet class
//
// Principal Author: Dmitry Bormotov,Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//
//


#include "OneVariableDataSet.h"
#include "QuickSort.h"
#include "math.h"
//#include "Random.h"


// ------------------------------- OneVariableDataSet --------------------------------- //
OneVariableDataSet::OneVariableDataSet( const DataSet& D, int index){ 
     double dValue;
     theSet=D.getSet(index);
    ///copy theSet to the Array
    theArray.reSize(theSet.length(),1);
    for( int index=0; index<theSet.length(); index++ ){
        theArray[index][0]=theSet[index];
    }
    sort();
}


void OneVariableDataSet::sort(){
   ////quick sort
   QuickSort(theArray,0,theArray.lastRow());
}

double OneVariableDataSet::mean( ) const{
  return sum() / cardinality();
}

double OneVariableDataSet::deviation( ) const{
  return pow(standardDeviation(),2);
}

double OneVariableDataSet::variance( ) const{
  double m = mean();
  double sum = 0;
  for( int index=0; index<theSet.length(); index++ )
    sum += fabs(theSet[index] - m);
  return sum;
}

double OneVariableDataSet::sum() const{
  double sum = 0;
  for( int index=0; index<theSet.length(); index++ )
    sum += theSet[index];
  return sum; 
}

double OneVariableDataSet::min() const{
   return theArray[0][0];
}
double OneVariableDataSet::max() const{
   return theArray[cardinality()-1][0];
}
int OneVariableDataSet::GetNum(double lower,double upper,bool upperIncluded) const{
  int num=0;
 
  for( int index=0; index<theSet.length(); index++ ){
   if(theSet[index]>=lower&&theSet[index]<upper)
       num++;
   if(theSet[index]==upper&&upperIncluded)
       num++;
  }

  return num; 
}

int OneVariableDataSet::cardinality() const{
   return theSet.length(); 
}

double OneVariableDataSet::standardDeviation() const{
  double m = mean();
  double sum = 0;
  for( int index=0; index<theSet.length(); index++ )
    sum += (theSet[index] - m) * (theSet[index] - m);
  if(cardinality()==1)
    return 0;
  return sqrt( sum / (cardinality() - 1) );
}

double OneVariableDataSet::range()const{
  return theArray[cardinality()-1][0]-theArray[0][0];
}

double OneVariableDataSet::median() const{
  return theArray[cardinality()/2][0];
}

double OneVariableDataSet::firstQuartile() const{
  return theArray[cardinality()/4][0];
}

double OneVariableDataSet::thirdQuartile() const{
  return theArray[cardinality()*3/4][0];
}

double OneVariableDataSet::coefficientSkewness() const{
  double U3 = 0;
  double m = mean();
  for( int index=0; index<theSet.length(); index++ )
     U3 += pow(theSet[index] - m ,3);

  return U3/pow(standardDeviation()*sqrt(cardinality()-1),3);
}

void OneVariableDataSet::printDefinition(ostream& ostr) const{
  ostr << "{";
  for(int i=0;i<cardinality();i++)
     ostr << theSet[i] << ' ';
  ostr << "}";
}

double OneVariableDataSet::getLogMean(){
  double sum=0;
  for(int i=0;i<cardinality();i++)
     sum+=log(theSet[i]);
  return sum/cardinality();
}

double OneVariableDataSet::getLogDeviation(){
  double sum=0;
  double logMean=getLogMean();
  for(int i=0;i<cardinality();i++)
     sum=sum+pow(log(theSet[i])-logMean,2);
  return sum;
}
