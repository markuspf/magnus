// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Global operators for Interprocess Communication (IPC).
//
// Principal Author: Dmitry Bormotov
//
// Status: Under development
//
// Special Notes:
//
//   This file includes only operators which will be used almost 
//   everywhere.
//

#ifndef _IPC_H_
#define _IPC_H_


// ------------------------- Output operators ------------------------------ //


inline std::ostream& operator < ( std::ostream& ostr, int n )
{
  ostr << n << ' ';
  return ostr;
}

inline std::ostream& operator < ( std::ostream& ostr, unsigned int n )
{
  ostr << n << ' ';
  return ostr;
}

inline std::ostream& operator < ( std::ostream& ostr, short n )
{
  ostr << n << ' ';
  return ostr;
}

inline std::ostream& operator < ( std::ostream& ostr, long n )
{
  ostr << n << ' ';
  return ostr;
}

inline std::ostream& operator < ( std::ostream& ostr, unsigned long n)
{
	ostr << n << ' ';
	return ostr;
}

inline std::ostream& operator < ( std::ostream& ostr, bool b )
{
  ostr << b << ' ';
  return ostr;
}

inline std::ostream& operator < ( std::ostream& ostr, char c )
{
  ostr << c << ' ';
  return ostr;
}

inline std::ostream& operator < ( std::ostream& ostr, const char *s )
{
  ostr << s << ' ';
  return ostr;
}

inline std::ostream& operator < ( std::ostream& ostr, const double d )
{
  ostr << d << ' ';
  return ostr;
}


// ------------------------- Input operators ------------------------------- //


inline std::istream& operator > ( std::istream& istr, int& n )
{
  istr >> n;
  return istr;
}

inline std::istream& operator > ( std::istream& istr, unsigned int& n )
{
  istr >> n;
  return istr;
}

inline std::istream& operator > ( std::istream& istr, short& n )
{
  istr >> n;
  return istr;
}

inline std::istream& operator > ( std::istream& istr, long& n )
{
  istr >> n;
  return istr;
}

inline std::istream& operator > ( std::istream& istr, bool& b )
{
  istr >> b;
  return istr;
}

inline std::istream& operator > ( std::istream& istr, char& c )
{
  istr >> c;
  return istr;
}

inline std::istream& operator > ( std::istream& istr, char* s )
{
  istr >> s;
  return istr;
}

inline std::istream& operator > ( std::istream& istr, double d )
{
  istr >> d;
  return istr;
}

inline std::istream& operator > ( std::istream& istr, void* p )
{
//  istr >> s;
  return istr;
}
//@db this function shouldn't be used at all, it just shuts the compiler up. 

#endif

