///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "Timer.h"
%}
class Timer
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Timer(int miliseconds) ;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool expired( ) const;

  void reset(int miliseconds);
/*

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC         :                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const Timer& T )
  {
    ostr < T.alarmSecs;
    ostr < T.alarmUSecs;
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, Timer& T)
  {
    istr > T.alarmSecs;
    istr > T.alarmUSecs;
    return istr;
  }
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  long alarmSecs, alarmUSecs;
*/
};


