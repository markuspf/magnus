// src/magnus.pamphlet
#ifndef _MAGNUSHOME_H_
#define _MAGNUSHOME_H_

#include "config.h"
#include "Chars.h"

struct MagnusHome
{
  friend int main(int argc, char* argv[]);
  static Chars magnusHome( ) { return Chars( magnus_home ); }

private:
  static char* magnus_home;
};

struct MagnusTmp
{
  friend int main(int argc, char* argv[]);
  static Chars magnusTmp( ) { return Chars( magnus_tmp ); }

private:
  static char* magnus_tmp;
};

#endif
