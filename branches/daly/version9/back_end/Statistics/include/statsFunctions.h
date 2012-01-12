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


#ifndef _STATSFUNCTIONS_H_
#define _STATSFUNCTIONS_H_

// ------------------------------ DataSet ---------------------------------- //

// We don't know yet if SetOf<double> is enough here, or we should use
// class template, i.e. DataSet<type>

const double ECONST=2.7182818284590452354;
const double PICONST=3.14159265358979323846;

double fmax(double x, double y);
double fmin(double x, double y);
class statsFunctions{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
 
  statsFunctions( ) { }
public:
  static double Norm(double x);
  static double NormInv(double x);
  static double TDis(double x,int n);
  static double TInv(double x,int n);
  static double Chi(double x,int n);
  static double ChiInv(double x,int n);
  static double Expo(double upper,double randa);
  static double Unif(double upper,double a,double b);
  static double FDis(double x, int n1, int n2);
  static double FInv(double y, int n1, int n2);
 };

#endif
