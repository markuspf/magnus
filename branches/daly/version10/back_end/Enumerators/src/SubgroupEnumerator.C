#include "SubgroupEnumerator.h"
#include "SMEnumerator.h"
#include "OutMessages.h"
#include "SMFPGroup.h"
#include "SMSubgroup.h"
#include "viewStructure.h"
#include "WordEnumerator.h"
#include "FreeGroup.h"
#include "SMWord.h"
#include "conversions.h"
#include "SMListIterator.h"

// ------------------- SGEnumeratorARCer --------------------- //


void SGEnumeratorARCer::enumerate( )
{
  //file <<  "Sample : " << theSample << endl;
  if (enumerateAll){
    if ( numberOfGenerators == 1 )
      enumerateAllSubgroups();
    else {
      WordEnumerator we(theGroup,numberOfGenerators);
      for (we.reset();counter < numberOfAll; we.next()) {
	if (submitSignalRecieved()){
	  return;
	}
	
	SetOf<Word> s = makeSetOf( we.getWords() );
	// Extract trivial words
	s -= Word();
	// Submit if non-empty set
	if ( s.cardinality() > 0 ) {
	  counter++;
	  theData << EnumWriteSetOfWords( s, theGroup) << "\n\n";
	}
      }
    }
  }else{
    while (counter < numberOfAll) {
      if (submitSignalRecieved()){
	return;
      }
      SetOf<Word> sw;
      for (int i1=0;i1<numberOfGenerators;i1++){
	sw |= getRandomWord();
      }
      // Extract trivial words
      sw -= Word();
      // Submit if non-empty set      
      if ( sw.cardinality() > 0 ) {
	counter++;
	theData << EnumWriteSetOfWords( sw, theGroup) << "\n\n";
      }      
    }
  }
}

void SGEnumeratorARCer::enumerateAllSubgroups( ) {
    
  for(;counter <numberOfAll; counter++){
    Word w = theGroup.getN_thElement( counter);
    if ( w.length() > 0 ) {
      counter++;
      SetOf<Word>  s;
      s |= w;
      theData << EnumWriteSetOfWords( s, theGroup) << "\n\n";
    }      
  }  
}

Word SGEnumeratorARCer::getRandomWord()  {
  int numOfGenerators = theGroup.numberOfGenerators()*2;
  int length = 0;
  if ( !isNormalRandom ) {
    length = ur.rand(1,maxLength);
  } else {
    while (length <=0 || length > maxLength)
      length = nr.rand(maxLength/2,maxLength/2);
  }
  
  Word newWord;
  
  for (int j=0;j<length;j++){
    int g=0;
    if ( !isNormalRandom ) {
      g = ur.rand(1,numOfGenerators);
    } else {
      while (g <=0 || g > numOfGenerators)
	g = nr.rand(numOfGenerators/2,numOfGenerators/2);
    }
    
    if ( g > numOfGenerators/2 ) {
      g = g - numOfGenerators/2;
    } else {
      g *= -1;
    }
    
    Generator newG(g);
    newWord *= newG;
  }
  return newWord.cyclicallyReduce();
}
// ---------------------- SGEnumeratorProblem --------------------- //

SGEnumeratorProblem::SGEnumeratorProblem(SMFPGroup& b)
  : EnumeratorProblem< Subgroup >( b ),
    theGroup(b),
    arcer( *this, theData,theGroup.getFreePreimage() )
{
  
}


void SGEnumeratorProblem::takeControl( )
{

  if (freeARCs() > 0){
    
    if (arcer.takeControl()){
      
      adminSuspend();
    return;	
    }
    else
      usedOneARC();
  }
}

void SGEnumeratorProblem::start( ) {              

  // Set arcer parameters
  int numOfGens = getParameters().getValue("NOfGens");
   // Set sample word
  arcer.setNumberOfGens( numOfGens );
 
  // Set total number of subgroup to enumerate
  int total = getParameters().getValue("NTotal");
  arcer.setTotalNumber( total );
  
  
  Chars t = getParameters().getValue("Method");
  // Random enumeration
  if (t == Chars("EnRandom")) {
    arcer.setEnumerateRandom();
    
    Chars t1 =  getParameters().getValue("RandomMethod");
    int maxLength;
    // Select normal or uniform enumeration is used
    if ( t1 == Chars("RBNormal")){
      arcer.setNormalRandom();
      maxLength = getParameters().getValue("maxLNormal");
    } else {
      arcer.setUniformRandom();
      maxLength = getParameters().getValue("maLUniform");
    }
    // Set maximal length of enumerated words
    arcer.setMaxLength( maxLength );
  }
  else 
    arcer.setEnumerateAll();
  
  LogMessage msg1(*this,theGroup);
  msg1 << Link(Chars( "Click here to see subgroups of ")+
	       Text( Name(theGroup) ),
	       "SGEnumerator",
	       theData.getDataFileName(),true );
  
  msg1.send();    
}

void SGEnumeratorProblem::resume( )
{              
 
   arcer.submitSignal();

   // Set arcer parameters
   
   // Set arcer parameters
   int numOfGens = getParameters().getValue("NOfGens");
   // Set sample word
   arcer.setNumberOfGens( numOfGens );
   
   // Set total number of words to enumerate
   int total = getParameters().getValue("NTotal");
   arcer.setTotalNumber( total );
   
   
   Chars t = getParameters().getValue("Method");
   // Random enumeration
   if (t == Chars("EnRandom")) {
     
     Chars t1 =  getParameters().getValue("RandomMethod");
     int maxLength;
     // Select normal or uniform enumeration is used
     if ( t1 == Chars("RBNormal")) {
       maxLength = getParameters().getValue("maxLNormal");
     } else {
       maxLength = getParameters().getValue("maLUniform");
     }
     // Set maximal length of enumerated words
     arcer.setMaxLength( maxLength );
   }
}

void SGEnumeratorProblem::viewStructure(ostream& ostr) const
{
  
   EnumertatorProblemView pv(oid(),"Subgroup enumerator",
			     Text("Enumerate subgroups in ") +
			     Name(theGroup) ,
			     helpID("SGEnumeratorProblem",theGroup),
			      Text("Sg en")
			     );

   pv.addParameter(EditInteger("NTotal","Number of subgroups",1,100,0,1000000));

   pv.addParameter(EditInteger("NOfGens","Max. number of generators of a subgroup",1,10,0,1000000));

   RadioButton rRB1("RBNormal","Enumerate using normal disribution");
   RadioButton rRB2("RBUniform","Enumerate using uniform disribution");

   rRB1.add(EditInteger("maxLNormal","Max. length of words",1,10,1,30000));
   rRB2.add(EditInteger("maLUniform","Max. length of words",1,10,1,30000));

   RadioButtonGroup randomRBG("RandomMethod");
   randomRBG.add(rRB1);
   randomRBG.add(rRB2);
      
   RadioButton rb1("EnRandom","Enumerate randomly");
   rb1.add(randomRBG);
   
   RadioButtonGroup rbg("Method");

   rbg.add(rb1);

   RadioButton rb2("EnTotal","Enumerate totally");
     
   rbg.add(rb2);

   pv.addParameter(rbg);




   pv.addProblem( Subproblem("Sp1",ThisARCSlotID(),
			     "For the use of enumerator",40));
   
  
   submit(ostr, pv);

}


// --------------------- IsSubgroupInListARCer ------------------------ //


void IsSubgroupInListARCer::setArguments( const SMList<Subgroup>& l, 
				      const VectorOf<Word>& s)
{
  theIterator = new SMListIterator<Subgroup>(l);
  theSubgroup = s;
}


void IsSubgroupInListARCer::runComputation( )
{
#if SAFETY > 0
  if( !theIterator )
    error( "void IsSubgroupInListARCer::runComputation( ) : "
	   "fileIterator must be initialized first");
#endif
   
  for (theIterator->reset();!theIterator->EOL();theIterator->nextCell()){
    SetOf<Word> s1 = makeSetOf( theSubgroup );
    SetOf<Word> s2 = makeSetOf( theIterator->getData().generators() );
    if ( s1 == s2 ){
      result = true;
      return;
    }
  }
  result = false;
}


void IsSubgroupInListARCer::writeResults( ostream& out ) 
{
  out < result;
}


void IsSubgroupInListARCer::readResults( istream& in )
{
  in > result;
}


bool IsSubgroupInListARCer::isInList() 
{
#if SAFETY > 0
  if( !theIterator )
    error( "bool IsSubgroupInListARCer::isInList() : "
	   "fileIterator must be initialized first");
#endif
  
  return result;
}

// ----------------------------- IsSubgroupInList ------------------------- //


IsSubgroupInList::IsSubgroupInList(SMList<Subgroup>& sml,
				   const SMSubgroup& sg)
  : Supervisor( true ),
    theList( sml ),
    theSubgroup( sg ),
    arcer(*this)
{ 
  arcer.setArguments(sml, sg.getSubgroup().generators() );
}


void IsSubgroupInList::takeControl( )
{
  if (freeARCs() > 0){
    
    if (arcer.takeControl()){
      LogMessage msg( *this, theSubgroup );
      msg << Name( theList );
      if (arcer.isInList())
	msg << " contains ";
      else
	msg << " does not contain ";
      
      msg << Name( theSubgroup ) << ".";
      
      msg.send();
      adminTerminate();
      return;
    } else
      usedOneARC();
  }
}



void IsSubgroupInList::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(),Text("Containment problem"),
		  Text("Determine whether") + Name(theSubgroup)
		  + Text("lies in") + Name(theList),
		  helpID("IsSubgroupInList", 
			 theList.getGroup()),
		  Text(Name(theSubgroup)) + Text("is in") +
		  Name(theList) + Text("?")
		  );
  
  pv.startItemGroup();

  pv.add( Text("For the use of this problem"), ThisARCSlotID(), 50 );
  pv.done();
}


// --------------------- SGListExtractOfIndexARCer -------------------- //


void SGListExtractOfIndexARCer::setArguments( const SMList<Subgroup>& l)
{
  theIterator  =  new SMListIterator<Subgroup>(l);
  theGroup = &(l.getGroup());
}


void SGListExtractOfIndexARCer::runComputation( )
{
#if SAFETY > 0
  if( ! theIterator )
    error( "void SGListExtractOgIndexARCer::runComputation( ) : "
	   "fileIterator must be initialized first");
#endif
  int counter = 1;
  theData.setWriteMode();

  for (theIterator->reset();!theIterator->EOL();theIterator->nextCell()){
    //while ( !theIterator->EOL() ) {
    int index;
    VectorOf<Word> v =  theIterator->getData().generators();
    
    if (theGroup->getCheckinType() == SMFPGroup::FREE )
      index = indexInFreeGroup( v );
    else
      index = indexInFPGroup( v );
    bool print = false;
    
    // if maximum index is infinite
    if ( maxIndex == 0 ){
      if ( index >= minIndex || index == 0)
	print = true;
    } else {
      if ( index >= minIndex && index <= maxIndex )
	print = true;
    }
    
    Chars charIndex;
    if (index != 0)
      charIndex = Chars( index );
    else
      charIndex = "infinite";

    if ( print ){
      theData << EnumWriteVectorOfWords( v, theGroup->getFPGroup() ) 
	      << Chars("\nIndex: ") 
	      << charIndex
	      << Chars(".\n\n");
      counter ++;
    }
    //theIterator->nextCell();
  }
  theData.closeCurrentMode();

}

int SGListExtractOfIndexARCer::indexInFreeGroup( const VectorOf<Word>& g )const
{
  SGofFreeGroup s( theGroup->getFreePreimage(), g );
  
  return s.findIndex();
}

int SGListExtractOfIndexARCer::indexInFPGroup( const VectorOf<Word>& s )const
{
  CosetEnumerator enumerator(theGroup->getFPGroup(),s);
  return enumerator.enumerate();
}

void SGListExtractOfIndexARCer::writeResults( ostream& out ) 
{
  out < theData;
}


void SGListExtractOfIndexARCer::readResults( istream& in )
{
  in > theData;
}


// ----------------------------- SGListExtractOfIndex  -------------------- //


SGListExtractOfIndex::SGListExtractOfIndex(SMList<Subgroup>& sml)
  : Supervisor( true ),
    theList( sml ),
    arcer(*this),
    started( false )
{ 
  arcer.setArguments(sml);
}

void SGListExtractOfIndex::start( ) {              

  int min = getParameters().getValue("minIndex");
  int max = getParameters().getValue("maxIndex");

  arcer.setMinimalIndex(min);
  arcer.setMaximalIndex(max);
  Chars maxIndex;
  if ( max == 0) 
    maxIndex = "infinite";
  else
    maxIndex = Chars(max);
  
  LogMessage msg( *this, theList );
  
  msg << Link(Chars( "Click here to see subgroups of index from ") +
	      Chars(min) +Chars(" to ") +
	      maxIndex + Chars(" in ") +
	      Text( Name( theList ) ),
	      "SGEnumerator",
	      arcer.getData().getDataFileName(),true );
  
  msg.send();   
}

void SGListExtractOfIndex::takeControl( )
{
  if (freeARCs() > 0){
    int min = getParameters().getValue("minIndex");
    int max = getParameters().getValue("maxIndex");
    Chars maxIndex;
    if ( max == 0) 
      maxIndex = "infinite";
    else
      maxIndex = Chars(max);
    
    if (arcer.takeControl()){
      SMList<Subgroup>* smo = 
	new  SMList<Subgroup>(theList.getGroup(),
			      arcer.getData(),
			      Text("Subgroups of index from ") +
			      Text(Chars(min)+Chars(" to ")+maxIndex+
				   Chars(" in ")) +
			      Name(theList)
			      ); 
	
      ListOf<OID> dependencies( smo->getGroup() );
      CheckinMessage( *smo, "", dependencies ).send();
      
      LogMessage msg( *smo );
      msg << Link( Chars("Click here to see the contents of ") + 
		   Text( Name( *smo) ),
		   "SGEnumerator",
		   smo->getData().getDataFileName() 
		   );
      msg << ".";
      msg.send();
      
      adminTerminate();
      return;
    }
    else
      usedOneARC();
  }
}



void SGListExtractOfIndex::viewStructure(ostream& ostr) const
{
  EnumertatorProblemView pv(oid(),"Extract subgroups of a given index from a list",
			    Text("Extract subgroups of a given index from ") +
			    Name( theList ) ,
			    helpID("SGListExtractOfIndex",
				   theList.getGroup()),
			    Text("index sgs")
			    );
  
  pv.addParameter(EditInteger("minIndex","Minimal index ",0,1,1,30000));
  pv.addParameter(EditInteger("maxIndex","Maximal index (0 - infinite)",0,0,0,30000));
  
  
  pv.addProblem( Subproblem("Sp1",ThisARCSlotID(),
			    "For the use of the problem",40));
  
  //  RadioButton rRB1("ToFile","Extract to a file");
  //  RadioButton rRB2("ToList","Extract to another list");
    
  //  RadioButtonGroup extractRBG("ExtractTo");
  
  //   extractRBG.add(rRB1);
  //   extractRBG.add(rRB2);

  //  pv.addParameter(extractRBG);
 
  submit(ostr, pv);

}



