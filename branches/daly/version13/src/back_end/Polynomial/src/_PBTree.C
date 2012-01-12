// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of class PBTree.
//
// Principal Author: Dmitry Bormotov
//
// Status: In development
//
// Usage:
//
// Discussion:
//
// Revision History:
//
// Next Implementation Steps:
//


#include "PBTree.h"
#include "Polynomial.h"


//--------------------------------- PBTree ----------------------------------//


template <class K>
bool PBTree<K>::search( const K& key, const PBTreePage<K>& searchPage,
		       PBTreePage<K> **keyPage, int& position )
{
  *keyPage = (PBTreePage<K> *)&searchPage;
  position = 0;
  if( (*keyPage)->numOfKeys == 0 )
    return false;
  while( true ) {                                  // cycle over pages
    position = 0;
    while( true ) {                                // cycle over positions
      int cmp = key.compare( *((*keyPage)->keys[position]) );
      if( cmp == 0 )
	return true;
      else
	if( cmp < 0 )
	  if( (*keyPage)->links[position] == NULL )
	    return false;
	  else {
	    (*keyPage) = (*keyPage)->links[position];
	    break;
	  }
	else {
	  ++position;
	  if( position >= (*keyPage)->numOfKeys )
	    if( (*keyPage)->links[position] == NULL )
	      return false;
	    else {
	      (*keyPage) = (*keyPage)->links[position];
	      break;
	    }
	}
    }
  }
}


template <class K>
void PBTree<K>::insert( const K& key )
{
  PBTreePage<K> *page;
  int position;
  if( search(key, *root, &page, position) ) {
    theKeyIsFound(key, *page, position);
  }
  else {
    PBTreePage<K> *leftLink = NULL;
    PBTreePage<K> *rightLink = NULL;
    K *insertKey = new K(key);

    while( true ) {

      bool isLeaf = ( leftLink == NULL );
      //int i;
      int numOfKeys = page->numOfKeys;

      // insert current key in current page

      if( numOfKeys < theOrder-1 ) { // if the page has a free place

        // simplest insertion: skiping keys and links
	// and puting new key on a free place

	for( int i = numOfKeys; i > position; --i )
	  page->keys[i] = page->keys[i-1];
	++(page->numOfKeys);
	page->keys[position] = insertKey;

	if( !isLeaf ) {
	  for( int i = numOfKeys; i > position; --i ) {
	    page->links[i+1] = page->links[i];
	    ++(page->links[i+1]->numOfParentLink);
	  }
	  page->links[position] = leftLink;
	  page->links[position+1] = rightLink;
	  leftLink->parentLink = page;
	  rightLink->parentLink = page;
	  leftLink->numOfParentLink = position;
	  rightLink->numOfParentLink = position+1;
	}
	break;  // finish the inserting
      }
      else {    // split the page

	// creating temporary keys and links with bigger sizes
	// ( really this is page with size equals order+1 )

	PBTreePage<K>* links[theOrder+1];
	K* keys[theOrder];
	int size = sizeof(keys);
	for( int i = 0; i < position; ++i )
	  keys[i] = page->keys[i];
	for( int i = position+1; i < theOrder; ++i )
	  keys[i] = page->keys[i-1];
	keys[position] = insertKey;
	
	if( !isLeaf ) {
	  for( int i = 0; i < position; ++i )
	    links[i] = page->links[i];
	  for( int i = position+1; i < theOrder; ++i )
	    links[i+1] = page->links[i];
	  links[position] = leftLink;
	  links[position+1] = rightLink;
	  leftLink->parentLink = page;
	  rightLink->parentLink = page;
	  leftLink->numOfParentLink = position;
	  rightLink->numOfParentLink = position+1;
	}
	
	// creating a brother of current page
	// and redistribution keys and links

	numOfKeys = theOrder/2;
	PBTreePage<K> *newBrotherPtr = new PBTreePage<K>(theOrder);

	page->numOfKeys = numOfKeys;
	
	int brotherNumOfKeys = theOrder-numOfKeys-1;
	newBrotherPtr->numOfKeys = brotherNumOfKeys;
	for( int i = 0; i < brotherNumOfKeys; ++i )
	  newBrotherPtr->keys[i] = keys[numOfKeys+1+i];

	if( !isLeaf ) 
	  for( int i = 0; i <= brotherNumOfKeys; ++i ) {
	    newBrotherPtr->links[i] = links[numOfKeys+1+i];
	    newBrotherPtr->links[i]->parentLink = newBrotherPtr;
	    newBrotherPtr->links[i]->numOfParentLink = i;
	  }
  	
        // go to the next iteration

	leftLink = page;
	rightLink = newBrotherPtr;
	insertKey = keys[numOfKeys];

	if( !page->parentLink ) {
	  page = new PBTreePage<K>(theOrder);
	  root = page;
	  position = 0;
	}
	else {
	  position = page->numOfParentLink;
	  page = page->parentLink;
	}
      }
    }
  }
}

template <class K>
bool PBTree<K>::Delete( const K& key )
{
  PBTreePage<K> *page;
  int position;
  if( search(key, *root, &page, position) ) {
    deleteKey(page, position);
    return true;
  }
  else
    return false;
}


template <class K>
void PBTree<K>::deleteKey( PBTreePage<K> *page, int position )
{
  bool isLeaf = ( page->links[0] == NULL );
  int minNumOfKeys = (theOrder - 1)/2;

  delete page->keys[position];

  if( !isLeaf ) {  // if it is not a leaf page

    // replace the key with its immediate successor

    PBTreePage<K> *oldPage = page;  
    page = page->links[position+1];
    while( page->links[0] )
      page = page->links[0];
    oldPage->keys[position] = page->keys[0];
    position = 0;
  }

  // remove the key from the leaf

  for( int i = position; i < page->numOfKeys-1; ++i )
    page->keys[i] = page->keys[i+1];
  --(page->numOfKeys);
  
  while( page->numOfKeys < minNumOfKeys ) {

    if( page == root ) return;
    
    // reorganize the tree

    PBTreePage<K> *parentLink = page->parentLink;
    int numOfParentLink = page->numOfParentLink;
    int parentNumOfKeys = parentLink->numOfKeys;
    int numOfKeys = page->numOfKeys;
    bool isLeaf = ( page->links[0] == NULL );
    
    PBTreePage<K> *leftSiblingLink = NULL, *rightSiblingLink = NULL;
    int leftNumOfKeys = 0, rightNumOfKeys = 0;
    
    if( numOfParentLink > 0 ) {
      leftSiblingLink = parentLink->links[numOfParentLink-1];
      leftNumOfKeys = leftSiblingLink->numOfKeys;
    }
    if( numOfParentLink < parentNumOfKeys ) {
      rightSiblingLink = parentLink->links[numOfParentLink+1];
      rightNumOfKeys = rightSiblingLink->numOfKeys;
    }

    // if the left sibling has unnecessary key
    // then redestribute it and quit the deletion

    if( leftNumOfKeys > minNumOfKeys ) {

      for( int j = numOfKeys; j > 0; --j )
	page->keys[j] = page->keys[j-1];
      page->keys[0] = parentLink->keys[numOfParentLink-1];
      parentLink->keys[numOfParentLink-1] =
	leftSiblingLink->keys[leftNumOfKeys - 1];
      
      if( !isLeaf ) {
	for( int j = numOfKeys+1; j > 0; --j ) {
	  page->links[j] = page->links[j-1];
	  ++(page->links[j]->numOfParentLink);
	}
	page->links[0] = leftSiblingLink->links[leftNumOfKeys];
	page->links[0]->numOfParentLink = 0;
	page->links[0]->parentLink = page;
      }
      ++(page->numOfKeys);
      --(leftSiblingLink->numOfKeys);

      return;
    }

    // if the right sibling has unnecessary key
    // then redestribute it and quit the deletion

    if( rightNumOfKeys > minNumOfKeys ) {

      page->keys[numOfKeys] = parentLink->keys[numOfParentLink];
      parentLink->keys[numOfParentLink] = rightSiblingLink->keys[0];
      for( int j = 0; j < rightNumOfKeys - 1; ++j )
	rightSiblingLink->keys[j] = rightSiblingLink->keys[j+1];
      
      if( !isLeaf ) {
	page->links[numOfKeys] = rightSiblingLink->links[0];
	page->links[numOfKeys]->numOfParentLink = numOfKeys;
	page->links[numOfKeys]->parentLink = page;
	for( int i = 0; i < rightNumOfKeys; ++i ) {
	  rightSiblingLink->links[i] = rightSiblingLink->links[i+1];
	  --(rightSiblingLink->links[i]->numOfParentLink);
	}
      }
      ++(page->numOfKeys);
      --(rightSiblingLink->numOfKeys);

      return;
    }

    // if the left sibling exist
    // then concatenate the page and the sibling,
    // demote their parent key into the combined page,
    // run this process for parent page
    
    if( leftSiblingLink ) {

      leftSiblingLink->keys[leftNumOfKeys] =
	parentLink->keys[numOfParentLink-1];
      for( int i = 0; i < numOfKeys; i++ )
	leftSiblingLink->keys[leftNumOfKeys+1+i] = page->keys[i];

      if( !isLeaf )
	for( int i = 0; i <= numOfKeys; i++ ) {
	  int num = leftNumOfKeys+1+i;
	  leftSiblingLink->links[num] = page->links[i];
	  leftSiblingLink->links[num]->parentLink = leftSiblingLink;
	  leftSiblingLink->links[num]->numOfParentLink = num;
	}
      
      leftSiblingLink->numOfKeys += numOfKeys+1;
      delete page;
      
      for( int i = numOfParentLink - 1; i < parentNumOfKeys - 1; ++i ) {
	parentLink->keys[i] = parentLink->keys[i+1];
	parentLink->links[i+1] = parentLink->links[i+2];
	--(parentLink->links[i+1]->numOfParentLink);
      }

      if( --(parentLink->numOfKeys) == 0 ) {
	delete parentLink;
	root = leftSiblingLink;
	return;
      }
      else {
	page = parentLink;
	continue;
      }
    }
    // if only the right sibling exist
    // then concatenate the page and the sibling,
    // demote their parent key into the combined page,
    // run this process for parent page
    
    page->keys[numOfKeys] = parentLink->keys[numOfParentLink];
    for( int i = 0; i < rightNumOfKeys; i++ )
      page->keys[numOfKeys+1+i] = rightSiblingLink->keys[i];
    
    if( !isLeaf )
      for( int i = 0; i <= rightNumOfKeys; i++ ) {
	int num = numOfKeys+1+i;
	page->links[num] = rightSiblingLink->links[i];
	page->links[num]->parentLink = page;
	page->links[num]->numOfParentLink = num;
      }
    
    page->numOfKeys += rightNumOfKeys+1;
    delete rightSiblingLink;
    
    for( int i = numOfParentLink; i < parentNumOfKeys - 1; ++i ) {
      parentLink->keys[i] = parentLink->keys[i+1];
      parentLink->links[i+1] = parentLink->links[i+2];
      --(parentLink->links[i+1]->numOfParentLink);
    }
    
    if( --(parentLink->numOfKeys) == 0 ) {
      delete parentLink;
      root = page;
      return;
    }
    else 
      page = parentLink;
  }  
}


template <class K>
void PBTree<K>::deleteAllPages( PBTreePage<K> *page )
{
  if( page == NULL )
    return;
  for( int i = 0; i < page->numOfKeys; ++i )
    delete page->keys[i];
  for( int i = 0; i <= page->numOfKeys; ++i )
    if( page->links[i] != NULL )
      deleteAllPages( page->links[i] );
  if( page != root )
    delete page;
  else
    page->numOfKeys = 0;
}


#ifdef DEBUG

template <class K>
void PBTree<K>::printAll( )
{
  PBTreeIterator<K> I(*this);
  for( ; !I.done(); I.next() ) {
    cout << "Page: ";
    for( int i = 0; i < I.page->numOfKeys; i++ )
      cout << *(I.page->keys[i]) << " ";
    for( int i = I.page->numOfKeys; i < I.tree.theOrder; i++ )
      cout << "  ";
    cout << " linkNumber: " << I.linkNumber
	 << " value: " <<  I.value() << endl;
  }
}

#endif


//---------------------------- PBTreeIterator -------------------------------//


template <class K>
void PBTreeIterator<K>::reset( )
{
  page = tree.root;
  linkNumber = 0;
  
  if( page->numOfKeys == 0 ) {          // it's an empty tree
    bDone = true;
    return;
  }

  bDone = false;

  // go down as more as possible

  while( page->links[linkNumber] )
    page = page->links[linkNumber];

  key = page->keys[linkNumber];         // set the value
}


template <class K>
bool PBTreeIterator<K>::next( )
{
#if SAFETY > 0
  if( bDone )
    error("K PBTreeIterator<K>::next( ) : "
	  "Tried to call next() when iterator is done.");
#endif
  
  ++linkNumber;                         // go right inside the page

  // go down as more as possible

  if( page->links[linkNumber] ) {
    while( page->links[linkNumber] ) {
      page = page->links[linkNumber];
      linkNumber = 0;
    }
  }

  while( linkNumber >= page->numOfKeys )  // while there are no more keys
                                          // in the page
    if( page->parentLink ) {              // if going up is possible
      linkNumber = page->numOfParentLink; // go up
      page = page->parentLink;
    }
    else {                                // else finish the iteration process
      bDone = true;
      return false;
    }
  key = page->keys[linkNumber];         // set the value
  return true;
}

     
//template class PBTree<Char>;
template class PBTree< Monomial<Integer> >;
template class PBTree< Monomial<Rational> >;
//template class PBTreeIterator<Char>;
template class PBTreeIterator< Monomial<Integer> >;
template class PBTreeIterator< Monomial<Rational> >;




