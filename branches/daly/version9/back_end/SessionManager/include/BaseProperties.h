//   Copyright (C) 1998 The New York Group Theory Cooperative
//   See magnus/doc/COPYRIGHT for the full notice.
 
// Contents: Definition of base properties.
//
// Principal Author: Dmitry Pechkin
//
// Status: In progress.
//
// Revision History:
//
// Special Notes:
//
// Further implementation steps:
//
// Bugs:
//
//

#ifndef _BASE_PROPERTIES_H_
#define _BASE_PROPERTIES_H_

#include "Property.h"
 
#include "Associations.h"
#include "QuickAssociations.h"
#include "Chars.h"
#include "Integer.h"

//============================================================
//
//          Simple properties:
//
//============================================================

class NoData {};
//  I/O stubs
inline istream& operator>>( const istream&, const NoData& ) {}
inline ostream& operator<<( const ostream&, const NoData& ) {}
 

class NoDataProperty : public SimpleProperty<NoData> {
public:
  NoDataProperty( const Chars& descr = Chars() ) 
    : SimpleProperty<NoData>(NoData(), descr) {}
};

class BoolProperty : public SimpleProperty<bool> {
public:
  BoolProperty( const bool& data, const Chars& descr = Chars() ) 
    : SimpleProperty<bool>(data, descr) {}
};

class IntProperty : public SimpleProperty<int> {
public:
  IntProperty( const int& data, const Chars& descr = Chars() ) 
    : SimpleProperty<int>(data, descr) {}
};



class IntegerProperty : public SimpleProperty<Integer> {
public:
  IntegerProperty( const Integer& data, const Chars& descr = Chars() ) 
    : SimpleProperty<Integer>(data, descr) {}
};


class SMFPCheckinTypeProperty : public SimpleProperty<int> {
public:
  SMFPCheckinTypeProperty( const int& data, const Chars& descr = Chars() ) 
    : SimpleProperty<int>(data, descr) {}
};


#endif
