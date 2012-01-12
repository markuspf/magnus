#include "AutoEnumerator.h"
#include "SMEnumerator.h"
#include "OutMessages.h"
#include "SMFPGroup.h"
#include "viewStructure.h"
#include <signal.h>


// ---------------------- AutEnumeratorARCer --------------------------- //


void AutEnumeratorARCer2::setArguments( FreeGroup group, 
					int avgNumbers,
					int n, 
					bool isfinite)
{
  if (randAuto)
    delete randAuto;
  randAuto = new RandomAutoInFree( group, avgNumbers );
  isFinite = isfinite;
  number = n;
}


void AutEnumeratorARCer2::enumerate( )
{
#if SAFETY > 0
  if( randAuto == 0 )
    error( "void AutEnumeratorARCer::runComputation( ) : "
	   "The enumerator has not been initialized yet");
#endif
  Map m;
  int order;
  
  while (current < number) {
    if (submitSignalRecieved()){
      return;
    }
    current++;
    if (!isFinite){
      m = randAuto->getAutomorphism();
      
    }
    else
      m = randAuto->getFiniteAutomorphism(order);
    file << current << ": ";
    m.printOn(file);
    if (isFinite){
      //      file.setColor(titleColor);
      file << endl << "The order: " << order;
      //      file.setColor(mainColor);
    }
    file << endl << endl;
  }
  
}


void AutEnumeratorARCer2::writeResults( ostream& out ) 
{
  out < current;
}


void AutEnumeratorARCer2::readResults( istream& in )
{
  in > current;
}


Chars AutEnumeratorARCer2::getFileName() const
{
  return file.getFileName();
}



// ---------------------- AutoEnumeratorProblem ----------------------- //

AutoEnumeratorProblem::AutoEnumeratorProblem(SMFPGroup& b)
  : EnumeratorProblem< Map >( b ),
    theGroup(b),
    arcer( *this )
{
}


void AutoEnumeratorProblem::takeControl( )
{

  if (freeARCs() > 0){

    if (arcer.takeControl()){
      adminSuspend();
      return;	
    }

    usedOneARC();
  }
  
}

void AutoEnumeratorProblem::viewStructure(ostream& ostr) const
{
  
   EnumertatorProblemView pv(oid(),"Automorphism enumerator",
			     Text("Randomly enumerate automorphisms of")+
			     Name( theGroup ),
			     helpID("AutoEnumeratorProblem",theGroup),
			      Text("Aut en")
			     );

   pv.addParameter(EditInteger("P1","Number of automorphisms",1,10,0,10000));
   pv.addParameter(EditInteger("P2","Average number of Nielsen generators",0,10,0,50));

   pv.addProblem( Subproblem("Sp1",ThisARCSlotID(),
			     "For the use of enumerator",40));
   
  
   submit(ostr, pv);

}

FiniteAutoEnumeratorProblem::FiniteAutoEnumeratorProblem(SMFPGroup& b)
  : EnumeratorProblem< Map >( b ),
    theGroup(b),
    arcer( *this )
{
}


void FiniteAutoEnumeratorProblem::takeControl( )
{

  if (freeARCs() > 0){

    if (arcer.takeControl()){
      adminSuspend();
      return;	
    }

    usedOneARC();
  }
  
}

void FiniteAutoEnumeratorProblem::viewStructure(ostream& ostr) const
{
 
  EnumertatorProblemView pv(oid(),"Automorphism enumerator",
			    Text("Randomly enumerate automorphisms of")
				 + Name(theGroup) 
				 + Text("of finite order"),
				 helpID("FiniteAutoEnumeratorProblem",
					theGroup),
				 Text("Fin aut en")
				 );
			    
   pv.addParameter(EditInteger("P1","Number of automorphisms",1,10,0,1000000));
   pv.addParameter(EditInteger("P2","Average number of Nielsen generators",0,10,0,50));

   pv.addProblem( Subproblem("Sp1",ThisARCSlotID(),
			     "For the use of enumerator",40));
   
  
   submit(ostr, pv);

}
