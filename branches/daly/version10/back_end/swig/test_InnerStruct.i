
%module Magnus
/*
%{
#include "test_InnerStruct.h"
%}

class Test
{
public:
   Test();
   TestStruct Fun();
   struct TestStruct{
       TestStruct(){
           x=0;
           y=0;
       }
       int x;
       int y;
   };
};
*/
// Handling of a simple nested structure


// This typemap is used to set the name field below

%typemap(memberin) char name[32] {
	strncpy($target,$source,31);
}	

%inline %{
typedef struct {  
                  unsigned int dataType;
                  union {
			int       intval;
			double    doubleval;
			char     *charval;
			void     *ptrvalue;
			long      longval;
			struct {
                               int    i;
			       double f;
                               void   *v;
  			       char name[32];

                        } v;
		} u;
} ValueStruct;


ValueStruct *new_value(unsigned int type) {
	ValueStruct *v;
	v = (ValueStruct *) malloc(sizeof(ValueStruct));
	v->dataType = type;
	return v;
}

%}