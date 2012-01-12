/*
 *   $Id: debug-Array.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents:
//
// Principal Author: Stephane Collart
//
// Status:
//
// Revision History:
//

#include "Array.h"


main ( ) {

char tab = '\t';
int i,j;

ArrayOf<int> a(2,3);

for (i = 0; i < a.numberOfRows(); i++)
	for (j = 0; j < a.numberOfColumns(); j++)
		a[i][j] = 10*i+j;

for (i = 0; i < a.numberOfRows(); i++) {
	cout << endl;
    for (j = 0; j < a.numberOfColumns(); j++)
		cout << tab << a[i][j];
}

}
