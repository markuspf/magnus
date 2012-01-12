// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Definition of the FPGroup class.
//
// Principal Authors: Alexey Myasnikov
//
// Status: in progress
//
// Revision History:
//
//
// Special Notes:
//
//

#include "AbelianSGPresentation.h"
#include "File.h"

Word AbelianSGPresentationRep::fromSGPGensToSGGens(const Word& w)const
{ 
    
     int numOfCanonGensInParent = theSGPGens.width();
     int numOfGensOfSGP = theSGPGens.height();
     int freePartOfParent = theParent.rankOfFreeAbelianFactor( );
     VectorOf<Integer> invariants = theParent.invariants();

     VectorOf<Integer> newPowers(numOfCanonGensInParent);
     VectorOf<Integer> givenWord = oldInAbelianForm(w).getPowers();
     AbelianWord retWord(theParent.getFPGroup().numberOfGenerators());

     for (int i=0;i<numOfGensOfSGP;i++){
        for (int j=0;j<numOfCanonGensInParent;j++)
          if (j<freePartOfParent)
             newPowers[j]=theSGPGens[i][j]*givenWord[i]; 
          else
              newPowers[j]=(theSGPGens[i][j]*givenWord[i])%invariants[j-freePartOfParent]; 
        retWord = retWord*theParent.newToOldGens(AbelianWord(newPowers));
    }
     return retWord.getWord();
}

bool AbelianSGPresentationRep::fromSGGensToSGPGens(const Word& w,Word& wInNew)const
{
       int numOfGensOfSGP = theSGPGens.height();
       int numOfParentCanonGens = theSGPGens.width(); 
       int freePartOfParent = theParent.rankOfFreeAbelianFactor( );

       VectorOf<Integer> givenWord = theParent.oldToNewGens(theParent.oldInAbelianForm(w)).getPowers();
       VectorOf<Integer> retPowers(numOfGensOfSGP);
       VectorOf<Integer> parentInvariants = theParent.invariants();
       int index=0;
       //Find the powers of SGP generators to present the given word
       for (int i=0;(i<numOfParentCanonGens)&&(index<numOfGensOfSGP);i++){ 
	 if (theSGPGens[index][i]==0 && givenWord[i]!=0) 
	   return false;
	 
	 if (theSGPGens[index][i]!=0){
	   // Powers for torsion free parts 
	   if (i<freePartOfParent){
	     if (powerOfNewGenInFreePart
		 (givenWord[i],theSGPGens[index][i],retPowers[index]))
	       for (int j=i;j<numOfParentCanonGens;j++){
                 givenWord[j]-=(retPowers[index]*theSGPGens[index][j]);
		 if (j>=freePartOfParent)
		  givenWord[j]= givenWord[j]%parentInvariants[j-freePartOfParent];
		 if (givenWord[j]<0 && j>=freePartOfParent)
		   givenWord[j]+=parentInvariants[j-freePartOfParent];
	       }
	     else
	       return false;              
	   }
	   // Powers for torsion parts 
	   else{
	     if (powerOfNewGenInTorsionPart
		 (parentInvariants[i-freePartOfParent],givenWord[i],
		  theSGPGens[index][i],retPowers[index])){
	       if (index <numOfGensOfSGP-1 )
		 for (int j=i;j<numOfParentCanonGens;j++){
		   givenWord[j]-=(retPowers[index]*theSGPGens[index][j])%
		     parentInvariants[j-freePartOfParent];
		   
		   if (givenWord[j]<0)
		     givenWord[j]+=parentInvariants[j-freePartOfParent];
		 }
	     }
	     else
	       return false;
	   }
	   index++;
	 }
       }

       bool isDone;
    // If the last generator espressed more than by one
    // canonical generator of parent group, we need to be shure
    // that it it's free or all torsion generators are equal to given one
    if (numOfGensOfSGP<numOfParentCanonGens && 
            numOfGensOfSGP > freePartOfParent ){
       Integer maxInvariant=0;
       for (int i=numOfGensOfSGP-1;i<numOfParentCanonGens;i++){
	 if (parentInvariants[i-freePartOfParent]>maxInvariant)
                       maxInvariant=parentInvariants[i-freePartOfParent];
       }
       Integer tmpInt;
       for (Integer i=0;i<maxInvariant;i+=1){
         isDone = true;
         for (int j=numOfGensOfSGP-1;j<numOfParentCanonGens;j++){
             tmpInt = ((retPowers[numOfGensOfSGP-1]+i*
                       parentInvariants[numOfGensOfSGP-1-freePartOfParent])*
                       theSGPGens[numOfGensOfSGP-1][j])%
                       parentInvariants[j-freePartOfParent];
	  if ((givenWord[j]-tmpInt)!=0){
	        isDone = false;
                break;
          }
         }
         if (isDone){
             retPowers[numOfGensOfSGP-1]+=i*parentInvariants[numOfGensOfSGP-1-freePartOfParent];
             wInNew = AbelianWord(retPowers).getWord();
             return true;
         }

       }
    }  	
    else{
      wInNew = AbelianWord(retPowers).getWord();
      return true;	
    }
    return false;
}
bool AbelianSGPresentationRep::powerOfNewGenInTorsionPart
                            (const Integer& inv,const Integer& b,const Integer& a,Integer& x)const
{
           bool noSolution = true;
           Integer res=0;
           while (res<inv){
              if (b%inv == (res*a)%inv){
                  noSolution = false;
                  x=res;
                  break;
              }
              res+=1;
           }
           //if no powers were found, return false
           if (noSolution)
                return false;
           return true;
}
bool AbelianSGPresentationRep::powerOfNewGenInFreePart
                            (const Integer& b,const Integer& a,Integer& x)const
{
           if (a==0)
	     if(b==0){
               x=0;
               return true;
             }
             else{
               return false;
             }
           if (b%a!=0)
                return false;
           x = b/a;
           return true;
}

void AbelianSGPresentationRep::makeFile( )
{
  File file;
  VectorOf<AbelianWord> cyclicGens = newToOldGens();
  int numofnewgens = cyclicGens.length();
  int rank = rankOfFreeAbelianFactor();
   char s[10];
   Word tmpWord;
   file.setColor(titleColor);
   file << "New generators of the subgroup expressed in terms of the given generators:" << endl;  
   file.setColor(mainColor);
   for( int i = 0; i < rank; ++i ) {
      file << "f" << i+1 << " = ";
      tmpWord =  fromSGPGensToSGGens(cyclicGens[i].getWord());
      theParent.getFPGroup().printWord(file,theParent.oldInAbelianForm(tmpWord).getWord() );
      file  << endl;
   }
  
  for( int i = rank; i < numofnewgens; ++i ) {
    file << "t" << i - rank + 1 << " = ";
    tmpWord =  fromSGPGensToSGGens(cyclicGens[i].getWord());
    theParent.getFPGroup().printWord(file,theParent.oldInAbelianForm(tmpWord).getWord() );
    file << endl;
  }
   file << endl;
   file.setColor(titleColor);
   file << "Given generators expressed in terms of the generators for the canonical decomposition of the subgroup:" << endl;  
   file.setColor(mainColor);
   for( int i = 0; i < theSGGens.length(); ++i ) {
    theParent.getFPGroup().printWord( file,theSGGens[i] );
    file << " = ";
    if (fromSGGensToSGPGens(theSGGens[i],tmpWord))
       printWordInNewGens(file,oldToNewGens(oldInAbelianForm(tmpWord)));   
       
    file << endl;
  } 
  file << end;
  theFileName = file.getFileName();
}

void AbelianSGPresentationRep::makeFileOfPDGens( )
{
  File file;
  char s[10];
  int rank = rankOfFreeAbelianFactor();
  VectorOf<Integer> newGens(numOfGensInTorsionPartOfPD()+rank);
  file.setColor(titleColor);
  file << "New generators of the subgroup expressed in terms of the given generators:" << endl;  
  file.setColor(mainColor);
  Word tmpWord;
  for( int i = 0; i < numOfGensInTorsionPartOfPD()+rank; ++i ) {
    if (i<rank)
       file << "f" << i+1 << " = ";
    else
       file << "p" << i+1-rank << " = ";
    newGens[i] = 1;  
    tmpWord =  fromSGPGensToSGGens(primeFormInOldGens( AbelianWord(newGens) ).getWord());
    theParent.getFPGroup().printWord(file,theParent.oldInAbelianForm(tmpWord).getWord() );
    newGens[i] = 0;
    file << endl;
  }
  file << endl;
   file.setColor(titleColor);
   file << "Given generators expressed in terms of the generators for the primary decomposition of the subgroup:" << endl;  
   file.setColor(mainColor);
  for( int i = 0; i < theSGGens.length(); ++i ) {
    theParent.getFPGroup().printWord( file,theSGGens[i] );
    file << " = ";
    if (fromSGGensToSGPGens(theSGGens[i],tmpWord))
         printInPrimaryForm(file,findEltPrimeForm(tmpWord));   
    file << endl;
  }    
  file << end;
  theFileNameOfPD = file.getFileName();
}

