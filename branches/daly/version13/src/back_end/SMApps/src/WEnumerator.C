#include "WEnumerator.h"
#include "SMEnumerator.h"
#include "OutMessages.h"
#include "SMFPGroup.h"
#include "viewStructure.h"
#include "WordEnumerator.h"
#include "FreeGroup.h"
#include "SMWord.h"

// ------------------- WordEnumeratorARCer --------------------- //


void WordEnumeratorARCer::enumerate( )
{
  //file <<  "Sample : " << theSample << endl;
  if (enumerateAll){
    if ( numberOfsampleGenerators == 1 )
      enumerateAllWords();
    else {
      WordEnumerator we(theGroup,numberOfsampleGenerators);
      for (we.reset();counter < numberOfAll; we.next()) {
	if (submitSignalRecieved()){
	  return;
	}
	
	VectorOf<Word> v = we.getWords();
	Word w = theSample.replaceGenerators( v );
	//	Word w = theSample.replaceGenerators( v ).freelyReduce();
	if ( w.length() > 0 ) {
	  counter++;
	  file << counter << ": ";
	  theGroup.printWord(file,w);
	  file << endl << endl;
	}
      }
    }
  }else{
    while (counter < numberOfAll) {
      if (submitSignalRecieved()){
	return;
      }
      VectorOf<Word> vw(numberOfsampleGenerators);
      for (int i1=0;i1<numberOfsampleGenerators;i1++){
	vw[i1] = getRandomWord();
      }
      Word w = theSample.replaceGenerators( vw ).freelyReduce();
      if ( w.length() > 0 ) {
	counter++;
	file << counter << ": ";
	theGroup.printWord(file,w);
	file << endl << endl;
      }      
    }
  }
}

void WordEnumeratorARCer::enumerateAllWords( ) {
    
  for(;counter <numberOfAll; counter++){
    Word w = theGroup.getN_thElement( counter );
    if ( w.length() > 0 ) {
      counter++;
      file << counter << ": ";
      theGroup.printWord(file,w);
      file << endl << endl;
    }      
  }  
}

Word WordEnumeratorARCer::getRandomWord()  {
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
  return newWord;
}
// ---------------------- WordEnumerator --------------------- //

WordEnumeratorProblem::WordEnumeratorProblem(SMFPGroup& b)
  : EnumeratorProblem< Word >( b ),
    theGroup(b),
    arcer( *this, theGroup.getFreePreimage() )
{
  
}


void WordEnumeratorProblem::takeControl( )
{

  if (freeARCs() > 0){
    //    arcer.runComputation();
    if (arcer.takeControl()){
      
      adminSuspend();
    return;	
    }
    else
      usedOneARC();
  }
}

void WordEnumeratorProblem::start( ) {              

  // Set arcer parameters
  Word sample = getParameters().getValue("Sample");
   // Set sample word
  arcer.setSample( sample );
 
  // Set total number of words to enumerate
  int total = getParameters().getValue("P1");
  arcer.setTotalNumber( total );
  
  
  Chars t = getParameters().getValue("RBG");
  // Random enumeration
  if (t == "EnRandom"){
    arcer.setEnumerateRandom();
    
    Chars t1 =  getParameters().getValue("RandomRBG");
    int maxLength;
    // Select normal or uniform enumeration is used
    if ( t1 == "RBNormal"){
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
  msg1 << Link(Chars( "Click here to see words in the generators of ")+
	       Text( Name(theGroup) ),
	       "WEnumerator",
	       arcer.getFileName(),true );
  
  msg1.send();    
  LogMessage msg(*this,theGroup);
  msg << sample;
  msg.send();    
  
}

void WordEnumeratorProblem::resume( )
{              
 
   arcer.submitSignal();

   // Set arcer parameters
   // Set total number of words to enumerate
   int total = getParameters().getValue("P1");
   arcer.setTotalNumber( total );
   
   
   Chars t = getParameters().getValue("RBG");
   // Random enumeration
   if (t == "EnRandom"){
     
     Chars t1 =  getParameters().getValue("RandomRBG");
     int maxLength;
     // Select normal or uniform enumeration is used
     if ( t1 == "RBNormal"){
       maxLength = getParameters().getValue("maxLNormal");
     } else {
       maxLength = getParameters().getValue("maLUniform");
     }
     // Set maximal length of enumerated words
     arcer.setMaxLength( maxLength );
   }
}

void WordEnumeratorProblem::viewStructure(ostream& ostr) const
{
  
   EnumertatorProblemView pv(oid(),"Word enumerator",
			     Text("Enumerate words in the generators of") +
			     Name(theGroup) ,
			     helpID("WEnumeratorProblem",theGroup),
			      Text("word en")
			     );

   pv.addParameter(EditInteger("P1","Number of words",1,100,0,1000000));


   VectorOf<Chars> v(1);
   v[0] = "x";
   FreeGroup f(v);
   Word w(Generator(1));
   pv.addParameter(EditWord("Sample","The generic word used in the enumeration",0,f,w));

   
   RadioButton rRB1("RBNormal","Enumerate using normal disribution");
   RadioButton rRB2("RBUniform","Enumerate using uniform disribution");

   rRB1.add(EditInteger("maxLNormal","Max. length of words",1,10,1,30000));
   rRB2.add(EditInteger("maLUniform","Max. length of words",1,10,1,30000));

   RadioButtonGroup randomRBG("RandomRBG");
   randomRBG.add(rRB1);
   randomRBG.add(rRB2);
      
   RadioButton rb1("EnRandom","Enumerate randomly");
   rb1.add(randomRBG);
   
   RadioButtonGroup rbg("RBG");

   rbg.add(rb1);

   RadioButton rb2("EnTotal","Enumerate totally");
     
   rbg.add(rb2);

   pv.addParameter(rbg);




   pv.addProblem( Subproblem("Sp1",ThisARCSlotID(),
			     "For the use of enumerator",40));
   
  
   submit(ostr, pv);

}


// --------------------- IsWordInListARCer ------------------------ //


void IsWordInListARCer::setArguments( const Chars& listFileName, 
				      const Word& w,
				      const FPGroup& g)
{
  fileName =  listFileName;
  theWord = w;
  theGroup = g;
}


void IsWordInListARCer::runComputation( )
{
#if SAFETY > 0
  if( fileName.length() == 0 )
    error( "void IsWordInListARCer::runComputation( ) : "
	   "fileIterator must be initialized first");
#endif
  SMListFileIterator<Word> I(fileName,theGroup);
  
  for (I.reset();!I.EOL();I.nextCell()){
    
    if ( theWord.freelyReduce() == I.getData() ){
      result = true;
      return;
    }
  }
  result = false;
}


void IsWordInListARCer::writeResults( ostream& out ) 
{
  out < result;
}


void IsWordInListARCer::readResults( istream& in )
{
  in > result;
}


bool IsWordInListARCer::isInList() 
{
#if SAFETY > 0
  if( fileName.length()  == 0 )
    error( "bool IsWordInListARCer::getCyclicDecomposition() : "
	   "fileIterator must be initialized first");
#endif
  
  return result;
}


// ----------------------------- IsWordInList ------------------------- //


IsWordInList::IsWordInList(SMList<Word>& sml,const SMWord& word)
  : Supervisor( true ),
    theList( sml ),
    theWord( word ),
    arcer(*this)
{ 
  arcer.setArguments(sml.getFileName(), word.getWord(), sml.getGroup().getFPGroup());
}


void IsWordInList::takeControl( )
{
  if (freeARCs() > 0){
    
    if (arcer.takeControl()){
      LogMessage msg( *this, theWord );
      msg << Name( theList );
      if (arcer.isInList())
	msg << " contains ";
      else
	msg << " does not contain ";
      msg << Name( theWord ) << ".";
      
      msg.send();
      adminTerminate();
      return;
    }
    else
      usedOneARC();
  }
}



void IsWordInList::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(),Text("Containment problem"),
		  Text("Determine whether") + Name(theWord)
		  + Text("lies in") + Name(theList),
		  helpID("IsWordInList", 
			 theList.getGroup()),
		  Text(Name(theWord)) + Text("is in") +
		  Name(theList) + Text("?")
		  );
  
  pv.startItemGroup();

  pv.add( Text("For the use of this problem"), ThisARCSlotID(), 50 );
  pv.done();
}

// --------------------- WordsOfLengthARCer ------------------------ //


void WordsOfLengthARCer::setArguments( const Chars& listFileName, 
				       const FPGroup& g)
{
  fileName =  listFileName;
  theGroup = g;
}


void WordsOfLengthARCer::runComputation( )
{
#if SAFETY > 0
  if( fileName.length() == 0 )
    error( "void WordsOfLengthARCer::runComputation( ) : "
	   "fileIterator must be initialized first");
#endif
  SMListFileIterator<Word> I(fileName,theGroup);
  int counter = 1;
  
  for (I.reset();!I.EOL();I.nextCell()){
    Word w = I.getData();
    if ( w.length() >= minLength && w.length() <= maxLength ){
      file << counter << ": ";
      theGroup.printWord(file, w );
      file << endl << endl;
      counter ++;
    }
  }
  
}


void WordsOfLengthARCer::writeResults( ostream& out ) 
{
  
}


void WordsOfLengthARCer::readResults( istream& in )
{
 
}


// ----------------------------- WordsOfLength ------------------------- //


WordsOfLength::WordsOfLength(SMList<Word>& sml)
  : Supervisor( true ),
    theList( sml ),
    arcer(*this),
    started( false )
{ 
  arcer.setArguments(sml.getFileName(), sml.getGroup().getFPGroup());
  
}


void WordsOfLength::takeControl( )
{
  if (freeARCs() > 0){
    if ( !started ){
      started = true;
      LogMessage msg( *this, theList );
      
      int min = getParameters().getValue("minL");
      int max = getParameters().getValue("maxL");
      
      msg << Link(Chars( "Click here to see the words of length from ")+Chars(min) +Chars(" to ") +
		  Chars(max) + Chars(" in ")+ Text( Name( theList ) ),
		  "WEnumerator",
		  arcer.getFileName(),true );
      
      msg.send();   
   }
    
    if (arcer.takeControl()){
      
      adminTerminate();
      return;
    }
    else
      usedOneARC();
  }
}



void WordsOfLength::viewStructure(ostream& ostr) const
{
  /*
  ProblemView pv( ostr, oid(),Text("Extract words of a given length"),
		  Text("Extract words of length from") + Name(theList),
		  helpID("WordsOfLength",
			 theList.getGroup()),
		  Text("words of len")
		  );
  
  pv.startItemGroup();

  pv.add( Text("For the use of this problem"), ThisARCSlotID(), 50 );
  pv.done();
  */

  EnumertatorProblemView pv(oid(),"Extract words of a given length from the list",
			    Text("Extract words from ") +
			    Name( theList ) ,
			    helpID("WordsOfLength",theList.getGroup()),
			    Text("words annulus")
			    );
  
  pv.addParameter(EditInteger("minL","Minimal length ",0,1,1,30000));
  pv.addParameter(EditInteger("maxL","Maximal length ",0,30000,1,30000));
  
  
  pv.addProblem( Subproblem("Sp1",ThisARCSlotID(),
			    "For the use of enumerator",40));
  
  
  submit(ostr, pv);

}

