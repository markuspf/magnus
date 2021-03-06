/*******************

Written by Frank Rimlinger, Jan 1993
*******************/
#include "crossTrainer.h"

#include "FuTreeInitInfo.h"
#include <stdio.h>
#include "FGLvar.h"
#include "FuTree.h"

FuTreeInitInfo::FuTreeInitInfo(void){

// NAME
	sprintf((char *)errorStr,"uTree"); 		// designation of GLvar for error handler
	CtoPstr((char*)errorStr);				  	// use Pascal version
// WALKS
	IDactiveMax=2;				// we use one slot for the walk and one for the
								// level number
// PACKET SIZES	
	rNoEntries=0;				// set by algorithm to number of
								// letters 	
	iNoEntries=1;
	lutNoEntries=1;				// ideal vertex look-up table
								
	ChildNoEntries=1064;		// child vertices generated by FGLwalkNode
	
// DATA
	useD=FALSE;
	DtypeSize=0;
	useiD=FALSE;
	iDtypeSize=0;

	
// RECYCLING
	recycleBlockSize=1064;
	recycleReal=TRUE;
	recycleIdeal=TRUE;
	
// BACKYARDS
	rBackYardSize=0;
	irBackYardSize=0;
	iBackYardSize=0;
	lutBackYardSize=0;
	
// the GLref is the externalRef and the breadCrumb is for tracing

	rvBackYardSize=sizeof(GLref)+sizeof(breadCrumb);
	
	ivBackYardSize=0;
}
