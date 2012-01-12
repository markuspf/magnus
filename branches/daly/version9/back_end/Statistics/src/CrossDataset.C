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


#include "CrossDataset.h"

// ------------------------------- CrossDataSet --------------------------------- //
CrossDataSet::CrossDataSet( const DataSet& D):m_Ds(D){
     m_pDs1=new OneVariableDataSet(D,0);
     if(D.getDimension()>1)
         m_pDs2=new OneVariableDataSet(D,1);
     else
         m_pDs2=NULL;
}

double CrossDataSet::GetX1(){
  return (*m_pDs1)[0];
}

double CrossDataSet::GetX2(){
  if(m_pDs2!=NULL)
      return (*m_pDs2)[0];
  else
      return 0;
}

double CrossDataSet::GetY1(){
  return (*m_pDs1)[1];
}

double CrossDataSet::GetY2(){
  if(m_pDs2!=NULL)
      return (*m_pDs2)[1];
  else
      return 0;
}

int CrossDataSet::GetRow(){
   m_pDs1->cardinality();
}

int CrossDataSet::GetColumn(){
   m_Ds.getDimension( );
}
double CrossDataSet::GetXSquare(){
  if(!CheckIntegrality())
      return 0;
  double T=GetT();
  double XSquare=0;
  for(int i=0;i<GetRow();i++){
      double Ri=GetRi(i);
      for(int j=0;j<GetColumn();j++){
          double Cj=GetCj(j);
          double Oij=GetOij(i,j);
          double Eij=Ri*Cj/T;
          double Itemij=pow(Oij-Eij,2)/Eij;
          XSquare+=Itemij;
      }
  }
  return XSquare;
}

bool CrossDataSet::CheckIntegrality(){
  int row=GetRow();
  for(int i=1;i<m_Ds.getDimension();i++){
     OneVariableDataSet* ds=new OneVariableDataSet(m_Ds,i);
     if(ds->cardinality()!=row)
         return false;
  }
  return true;
}

double CrossDataSet::GetRi(int i){
  double T=0;
  for(int j=0;j<GetColumn();j++){
    OneVariableDataSet* ds=new OneVariableDataSet(m_Ds,j);
    T+=(*ds)[i];
  }
  return T;
}

double CrossDataSet::GetCj(int j){
  double T=0;
  OneVariableDataSet* ds=new OneVariableDataSet(m_Ds,j);
  for(int i=0;i<GetRow();i++)
      T+=(*ds)[i];
  return T;
}

double CrossDataSet::GetT(){
  double T=0;
  for(int j=0;j<GetColumn();j++){
    OneVariableDataSet* ds=new OneVariableDataSet(m_Ds,j);
    for(int i=0;i<GetRow();i++)
      T+=(*ds)[i];
  }
  return T;
}

double CrossDataSet::GetOij(int i,int j){
     OneVariableDataSet* ds=new OneVariableDataSet(m_Ds,j);
     return (*ds)[i];
}
