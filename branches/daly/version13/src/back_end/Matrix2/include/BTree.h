// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definitions of class BTree.
//           
// Principal Author: Dmitry Bormotov
//
// Status: In development
//
// Usage:
//
// Revision History:
//


#ifndef _BTREE_H_
#define _BTREE_H_

#include "Char.h"

//------------------------------- BTreePage ---------------------------------//


template <class K> class BTreePage
{

  friend class BTree<K>;
  friend class BTreeIterator<K>;

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  BTreePage( int order ) : theOrder( order ), numOfKeys( 0 ), 
      parentLink( NULL )
  {
  #if SAFETY > 0
    if( order < 2 )
      error("template <class K> class BTree::BTree( int ) : "
	    "the order of BTree must be 2 or more.");
  #endif  
    
    keys = new (K*)[order-1];
    links = new (BTreePage<K>*)[order];
    for( int i = 0; i < order; ++i )
      links[i] = NULL;
  }

  ~BTreePage( ) 
  { 
    delete [] links;
    delete [] keys;
  }


private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int theOrder;
  int numOfKeys;
  K **keys;
  BTreePage **links;
  BTreePage *parentLink;
  int numOfParentLink;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  friend int main( );

#endif

};


//--------------------------------- BTree -----------------------------------//


template <class K> class BTree
{

  friend class BTreeIterator<K>;

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  BTree( int order = 5 ) : theOrder( order )
  {
    #if SAFETY > 0
      if( order < 2 )
	error("template <class K> class BTree::BTree( int ) : "
	      "the order of BTree must be 2 or more.");
    #endif  
      
    root = new BTreePage<K>(order);
  }

  BTree( const BTree& );

  ~BTree( ) 
  { 
    deleteAll(); 
    delete root;
  }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool Delete( const K& key );

  void insert( const K& key );
    

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O:                                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
/*
  friend ostream& operator << ( ostream& ostr, const Matrix& M )
  {
    M.printOn(ostr);
    return ostr;
  }

*/

protected:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Protected functions:                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  virtual void theKeyIsFound( BTreePage<K>& keyPage, int position ) { }

  bool search( const K& key, const BTreePage<K>& searchPage,
	       BTreePage<K> **keyPage, int& position );
  
  void deleteKey( BTreePage<K> *page, int position );

  void deleteAll( ) { deleteAllPages(root); }

  void deleteAllPages( BTreePage<K> *page );


private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int theOrder;
  BTreePage<K> *root;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


/*
  void printOn( ostream& ) const;
*/

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  void printAll( );

  friend int main( );

#endif

};


//----------------------------- BTreeIterator -------------------------------//

// Note: This iterator don't make a copy of BTree. Be careful - don't 
// delete keys of tree when iterating over them.


template <class K> class BTreeIterator 
{

  friend class BTree<K>; 

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  BTreeIterator( const BTree<K>& T ) : tree( T ) { reset( ); }

  // No default constructor
  // Copy constructor provided by compiler (does logical deep copy).


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Status Queries:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool done( ) { return bDone; }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void reset( );

  K value( ) 
  { 
  #if SAFETY > 0
    if( bDone )
      error("K BTreeIterator<K>::value( ) : "
	    "Tried to retrieve value from iterator which is done.");
  #endif  
    
    return *key; 
  }

  bool next( );


private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const BTree<K>& tree; 
  BTreePage<K> *page;  // current page in tree 
  int linkNumber;    // current link in tree
  bool bDone;
  K *key;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
};

#endif





