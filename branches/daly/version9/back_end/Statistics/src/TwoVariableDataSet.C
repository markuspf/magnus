// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of TwoVariableDataSet class
//
// Principal Author: Dmitry Bormotov,Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//
//


#include "TwoVariableDataSet.h"
//#include "Random.h"


// ------------------------------- TwoVariableDataSet --------------------------------- //
TwoVariableDataSet::TwoVariableDataSet( const DataSet& D){
     m_pDs1=new OneVariableDataSet(D,0);
     m_pDs2=new OneVariableDataSet(D,1);
}

bool TwoVariableDataSet::IsEqualSize(){
  return m_pDs1->cardinality()==m_pDs2->cardinality();
}

///        =Sxy/Sxx

double TwoVariableDataSet::GetSlope(){
  double Sxy=0;
  int size=m_pDs1->cardinality();
  if(m_pDs1->deviation()==0)
    return 0;
  else
    return GetSxy()/(m_pDs1->deviation()*(size-1));
}
double TwoVariableDataSet::GetIntercept(){
  return m_pDs2->mean()-m_pDs1->mean()*GetSlope();
}

double TwoVariableDataSet::GetSxy(){
  double Sxy=0;  
  int size=m_pDs1->cardinality();
  if(!IsEqualSize())
    return 0;
  for(int i=0;i<size;i++)
    Sxy+=((*m_pDs1)[i]-m_pDs1->mean())*((*m_pDs2)[i]-m_pDs2->mean());
  return Sxy;
}
double TwoVariableDataSet::GetTValue(){
   double Se=GetSe();
   int size=m_pDs1->cardinality();
   if(Se==0)
     return 0;
   else
     return GetSlope()*sqrt(size-1)*m_pDs1->standardDeviation()/Se;
}

double TwoVariableDataSet::GetR(){
   int size=m_pDs1->cardinality();
   double SSE=m_pDs2->deviation()*(size-1)-GetSxy()*GetSlope();
   double Syy=(size-1)*m_pDs2->deviation();
   if(Syy==0)
     return 0;
   else
     return sqrt((Syy-SSE)/Syy);
}
///get the sum of ranks for x data 
double TwoVariableDataSet::GetRx(){
   int m=m_pDs1->cardinality();
   double sum=0;
   for(int i=0;i<m;i++) {
     int RankX=m_pDs1->GetNum(m_pDs1->min(),(*m_pDs1)[i]);
     int RankY=m_pDs2->GetNum(m_pDs2->min(),(*m_pDs1)[i]);
     int exqualX=m_pDs1->GetNum((*m_pDs1)[i],(*m_pDs1)[i],true);
     int exqualY=m_pDs2->GetNum((*m_pDs1)[i],(*m_pDs1)[i],true);
     double rank=RankX+RankY+(exqualX+exqualY+1.0)/2.0;
     sum+=rank;
   }
   return sum;
}

double TwoVariableDataSet::GetUx(){
   int m=m_pDs1->cardinality();
   double Ux=GetRx()-m*(m+1.0)/2.0;
   return Ux;
}

double TwoVariableDataSet::GetUy(){
   int m=m_pDs1->cardinality();
   int n=m_pDs2->cardinality();
   double Uy=(m+n)*(m+n+1.0)/2.0-GetRx()-n*(n+1.0)/2.0;
   return Uy;  
}

double TwoVariableDataSet::GetSe(){
   int size=m_pDs1->cardinality();
   double SSE=m_pDs2->deviation()*(size-1)-GetSxy()*GetSlope();
   double Se=sqrt(SSE/(size-2.0));
   return Se;
}

///     =Se/sqrt(Sxx)
double TwoVariableDataSet::GetSb(){
    double Se=GetSe();
    int size=m_pDs1->cardinality();
    double Sxx=(size-1)*m_pDs1->deviation();
    if(Sxx==0)
       return 0;
    else
       return Se/sqrt(Sxx);
}        

///      =Se*sqrt(1/n+X^2/Sxx)
double TwoVariableDataSet::GetSa(){
    double Se=GetSe();
    int size=m_pDs1->cardinality();
    double Sxx=(size-1)*m_pDs1->deviation();
    double X=m_pDs1->mean();
    if(Sxx==0)
       return 0;
    else
       return Se*sqrt(1.0/size+X*X/Sxx);
}

int TwoVariableDataSet::GetSize(){
    int size=m_pDs1->cardinality();
    return size;
}

double TwoVariableDataSet::GetSUy(double X0){
    double Se=GetSe();
    int size=m_pDs1->cardinality();
    double Sxx=(size-1)*m_pDs1->deviation();
    double X=m_pDs1->mean();
    if(Sxx==0)
       return 0;
    else
       return Se*sqrt(1.0/size+(X0-X)*(X0-X)/Sxx);
}

double TwoVariableDataSet::GetSpred(double X0){
    double Se=GetSe();
    int size=m_pDs1->cardinality();
    double Sxx=(size-1)*m_pDs1->deviation();
    double X=m_pDs1->mean();
    if(Sxx==0)
       return 0;
    else
       return Se*sqrt(1+1.0/size+(X0-X)*(X0-X)/Sxx);
}
double TwoVariableDataSet::GetSxLogy(){
  double sum=0;
  double meanX=m_pDs1->mean();
  double meanLogY=m_pDs2->getLogMean();
  for(int i=0;i<GetSize();i++)
     sum=sum+((*m_pDs1)[i]-meanX)*(log((*m_pDs2)[i])-meanLogY);
  return sum;
}
double TwoVariableDataSet::GetSLogxLogy(){
  double sum=0;
  double meanLogX=m_pDs1->getLogMean();
  double meanLogY=m_pDs2->getLogMean();
  for(int i=0;i<GetSize();i++)
     sum=sum+(log((*m_pDs1)[i])-meanLogX)*(log((*m_pDs2)[i])-meanLogY);
  return sum;
}

double TwoVariableDataSet::GetSLogxLogx(){
  double sum=0;
  double meanLogX=m_pDs1->getLogMean();
  for(int i=0;i<GetSize();i++)
     sum=sum+pow((log((*m_pDs1)[i])-meanLogX),2);
  return sum;
}

double TwoVariableDataSet::GetLogA(){
  double a=m_pDs2->getLogMean()-log(GetLogB())*m_pDs1->mean();
  return exp(a);
}

double TwoVariableDataSet::GetLogB(){
  double Sxx=(GetSize()-1)*m_pDs1->deviation();
  double b=GetSxLogy()/Sxx;
  return exp(b);
}
double TwoVariableDataSet::GetLogLogA(){
  double a=m_pDs2->getLogMean()-GetLogLogB()*m_pDs1->getLogMean();
  return exp(a);
}
double TwoVariableDataSet::GetLogLogB(){
  double Sxx=(GetSize()-1)*m_pDs1->deviation();
  double b=GetSLogxLogy()/GetSLogxLogx();
  return b;
}

double TwoVariableDataSet::getQSqure(double A,double B){
  double sum=0;
  double Syy=(GetSize()-1)*m_pDs2->deviation();
  for(int i=0;i<GetSize();i++)
     sum=sum+pow((*m_pDs2)[i]-A*pow(B,(*m_pDs1)[i]),2);
  return (Syy-sum)/Syy;
}

double TwoVariableDataSet::getPSqure(double A,double B){
  double sum=0;
  double Syy=(GetSize()-1)*m_pDs2->deviation();
  for(int i=0;i<GetSize();i++)
     sum=sum+pow(((*m_pDs2)[i]-A*pow((*m_pDs1)[i],B)),2);
  return (Syy-sum)/Syy;
}
