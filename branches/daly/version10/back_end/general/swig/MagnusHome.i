////created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "MagnusHome.h"
%}

struct MagnusHome
{
///  friend int main(int argc, char* argv[]);
  Chars magnusHome( );
/*
private:
  static char* magnus_home;
public:
  %addmethods{
    void SetHome(char* strHome){
      magnus_home=strHome;
    }
  }
*/
};

