// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMDataSetComputation
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//

#include "statsFunctions.h"
#include "SMCrossDatasetProblems.h"
#include "CrossDataset.h"
#include "OutMessages.h"

void SMCrossDataSetConfidenceOne::sendResult( ) const{
  CrossDataSet ds(theDataSet.getSet());
  {    LogMessage msg( oid(), 0);
       msg <<  "      CONFIDENCE INTERVAL FOR ONE PROPORTION " ;
       msg << "Level:" << m_Level;
       msg.send();
  }
  double X=ds.GetX1();
  double N=ds.GetY1();
  double p=X/N;
  double Z=statsFunctions::NormInv(0.5+m_Level/2);
  double A=p-Z*sqrt(p*(1-p)/N);
  double B=p+Z*sqrt(p*(1-p)/N);
  {    LogMessage msg( oid(), 0);
       msg <<  " There were " << X << " successes out of a total of " << N << "trials" ;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " This was a sample proportion of " << p;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " A " << 100*m_Level << "% confidence interval for the corresponding population "
           << "proportion p is";
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "      " << A << " <= p <= " << B ;
       msg.send();
  }
}

void SMCrossDataSetHypothesisOne::sendResult( ) const{
  CrossDataSet ds(theDataSet.getSet());
  {    LogMessage msg( oid(), 0);
       msg <<  "      HYPOTHESIS TESTING FOR ONE PROPORTION " ;
       msg << "Level:" << m_Level 
           << "MU0:" << m_MU0
           << "Type:" << m_Type;
       msg.send();
  }
  double X=ds.GetX1();
  double N=ds.GetY1();
  double p=X/N;
  double Z1=(p-m_MU0)/sqrt(m_MU0*(1-m_MU0)/N);
  double PV;
  Chars strType;
  switch (m_Type){
    case '1': PV=1-statsFunctions::Norm(abs(Z1)); strType="p!=p0"; break;
    case '2': PV=1-statsFunctions::Norm(Z1); strType="p>p0";break;
    case '3': PV=statsFunctions::Norm(Z1); strType="p<p0"; break;

    default: PV=0;
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " There were " << X << " successes out of a total of " << N << "trials" ;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " This was a sample proportion of " << p;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "   The null hypothisis was p=" << m_MU0 << " and the alternative was ";
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "         " << strType;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "   There was a P-value of "<< PV;
       msg.send();
  }
  Chars isSign="was";
  if(PV>m_Level)
       isSign="was not";
  {    LogMessage msg( oid(), 0);
       msg <<  "   This "<<isSign<<" significant at a "<< 100*m_Level << "% level of significance.";
       msg.send();
  }
}

void SMCrossDataSetConfidenceTwo::sendResult( ) const{
  CrossDataSet ds(theDataSet.getSet());
  {    LogMessage msg( oid(), 0);
       msg <<  "      CONFIDENCE INTERVAL FOR TWO PROPORTIONS " ;
       msg << "Level:" << m_Level;
       msg.send();
  }
  double X1=ds.GetX1();
  double N1=ds.GetY1();
  double X2=ds.GetX2();
  double N2=ds.GetY2();
  double p1=X1/N1;
  double p2=X2/N2;
  double p3=abs(p1-p2);
  double Z=1-statsFunctions::Norm(m_Level/2);
  double A1=p1-Z*sqrt(p1*(1-p1)/N1);
  double B1=p1+Z*sqrt(p1*(1-p1)/N1);
  double A2=p2-Z*sqrt(p2*(1-p2)/N2);
  double B2=p2+Z*sqrt(p2*(1-p2)/N2);
  double S=sqrt(p1*(1-p1)/N1+p2*(1-p2)/N2);
  double A3=p3-Z*S;
  double B3=p3+Z*S;
  {    LogMessage msg( oid(), 0);
       msg <<  " In the first sample there were " << X1 << " successes out of a total of " << N1 << " trials" ;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " This was a sample proportion of " << p1;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " A " << 100*m_Level << "% confidence interval for the corresponding population "
           << "proportion p1 is";
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "      " << A1 << " <= p1 <= " << B1 ;
       msg.send();
  }

  {    LogMessage msg( oid(), 0);
       msg <<  " In the second sample there were " << X2 << " successes out of a total of " << N2 << " trials" ;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " This was a sample proportion of " << p2;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " A " << 100*m_Level << "% confidence interval for the corresponding population "
           << "proportion p2 is";
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "      " << A2 << " <= p2 <= " << B2 ;
       msg.send();
  }

  {    LogMessage msg( oid(), 0);
       msg <<  " A " << 100*m_Level << "% confidence interval for the defference of the two population "
           << "proportions |p1-p2| is ";
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "      " << A3 << " <= |p1-p2| <= " << B3 ;
       msg.send();
  }


}

void SMCrossDataSetHypothesisTwo::sendResult( ) const{
  CrossDataSet ds(theDataSet.getSet());
  {    LogMessage msg( oid(), 0);
       msg <<  "      HYPOTHESIS TESTING FOR TWO PROPORTIONS " ;
       msg << "Level:" << m_Level 
           << "MU0:" << m_MU0
           << "Type:" << m_Type;
       msg.send();
  }
  double X1=ds.GetX1();
  double N1=ds.GetY1();
  double X2=ds.GetX2();
  double N2=ds.GetY2();
  double p1=X1/N1;
  double p2=X2/N2;
  double p3=p1-p2;
  double S=sqrt(p1*(1-p1)/N1+p2*(1-p2)/N2);
  double Z1=(p3-m_MU0)/S;

  double PV;
  Chars strType;
  switch (m_Type){
    case '1': PV=1-statsFunctions::Norm(abs(Z1)); strType="p1-p2!=p0"; break;
    case '2': PV=statsFunctions::Norm(Z1); strType="p1-p2<p0"; break;
    case '3': PV=1-statsFunctions::Norm(Z1); strType="p1-p2>p0";break;
    default: PV=0;
  }

  {    LogMessage msg( oid(), 0);
       msg <<  " In the first sample there were " << X1 << " successes out of a total of " << N1 << " trials" ;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " This was a sample proportion of " << p1;
       msg.send();
  }

  {    LogMessage msg( oid(), 0);
       msg <<  " In the second sample there were " << X2 << " successes out of a total of " << N2 << " trials" ;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " This was a sample proportion of " << p2;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " The null hypothesis was p1-p2=" << m_MU0 << " and the alternate was";
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "         " << strType;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "   There was a P-value of "<< PV;
       msg.send();
  }
    
  Chars isSign="was";
  if(PV>m_Level)
       isSign="was not";
  {    LogMessage msg( oid(), 0);
       msg <<  "   This "<<isSign<<" significant at a "<< 100*m_Level << "% level of significance.";
       msg.send();
  }

}

void SMCrossDataSetTableAnalysis::sendResult( ) const{
  CrossDataSet ds(theDataSet.getSet());
  double P;
  double X=ds.GetXSquare();
  P=1-statsFunctions::Chi(X,(ds.GetRow()-1)*(ds.GetColumn()-1));
  {    LogMessage msg( oid(), 0);
       msg <<  "      CONTINGENCY TABLE ANALYSIS " ;
       msg << "Level:" << m_Level 
           << "Row factor:" << m_RowFactor
           << "Column factor:" << m_ColumnFactor;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "  The chi-square value for this contingency table was " << X;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "  This table was " << ds.GetRow() << "*" << ds.GetColumn()
           << " and therefore there were " << (ds.GetRow()-1)*(ds.GetColumn()-1)
           << " degrees of freedom";
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "  The row factor was " << m_RowFactor;
       msg.send();
  }

  {    LogMessage msg( oid(), 0);
       msg <<  "  The column factor was " << m_ColumnFactor;
       msg.send();
  }
  {    LogMessage msg( oid(), 0);
       msg <<  "  The computed P value was " << P;
       msg.send();
  }
  Chars isSign=" are ";
  Chars isIndep=" is ";
  if(P>m_Level){
      isSign=" are not ";
      isIndep=" is not ";
  }
  {    LogMessage msg( oid(), 0);
       msg <<  " Hence the result " << isSign << " significant at a "<< 100*m_Level
           << "% level. Therefore " << m_RowFactor << isIndep
           << " independent from " << m_ColumnFactor;
       msg.send();
  }
}
