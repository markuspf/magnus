// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Informal tests of class BTree
//
// Principal Authors: Dmitry Bormotov
//


#include "global.h"
#include "BTree.h"


void main( )
{
  /*
  const numOfElt = 17;
  const char Elt[numOfElt+1] = "CAKRMGOTDXSLJZNPW";

  BTree<int,char> T;
  for( int i = 0; i < numOfElt; ++i )
    T.insert(Elt[i], Elt[i]);

  T.remove(Elt[4]);
  T.printAll();
  cout << endl;

  int key = Elt[2];
  char* value = T.search(key);
  cout << "(" << key << "," << *value << ")" << endl;
  */

  /*
  BTree<int,int> T(3);
 
  T.insert(5, 50);
  T.printAll();
  cout << endl;
  
  T.insert(4, 40);
  T.printAll();
  cout << endl;
 
  T.insert(3, 30);
  T.printAll();
  cout << endl;
 
  T.insert(2, 20);
  T.printAll();
  cout << endl;
 
  T.insert(1, 10);
  T.printAll();
  cout << endl;
  
  T.remove(2);
  
  T.printAll();
  cout << endl;

  for( int key = 1; key <= 5; ++key ) {
    int* pValue = T.search(key);
    if( pValue ) 
      cout << "(" << key << "," << *pValue << ")" << endl;
    else
      cout << "The key " << key << " is not found."<< endl;
  }
  
  cout << endl;
  */
  BTree<int,int> T(4);
  T.insert(1,1);
  T.insert(2,2);
  T.insert(3,3);
  T.insert(4,4);
  T.insert(5,5);
  T.insert(6,6);
  T.insert(7,7);
  T.remove(3);
  T.insert(1,3);
  //T.remove(4);
  T.insert(2,4);
  //T.remove(5);
  T.insert(1,5);
  //T.remove(6);
  T.insert(2,6);
  //T.remove(2);
  //T.remove(7);
  T.insert(1,7);
  T.insert(8,8);
  T.insert(9,9);
  T.insert(10,10);
  T.insert(11,11);
  T.insert(12,12);
  T.printAll();
}


