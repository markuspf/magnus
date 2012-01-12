#include "SMBaseOfDataSet.h"

void registerProperties()
{
  PropertiesManager* pm = PropertiesManager::instance();

  //----------------------------------------------------------------------
  //
  //  DIC
  //
  //----------------------------------------------------------------------
  
  {
    pm -> registerProperty( IsOneVariableDataSetProperty (                ) );
    pm -> registerProperty( IsTwoVariableDataSetProperty (                ) );
  }

}
